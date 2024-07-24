#include "ParticleSystem.h"
#include "RandomGenerator.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <exception>
#include "raylib.h"
#include "raymath.h"
#include "transformation_stack.h"
#include <iostream>

RandomGenerator rng;

float calculate_variation(float inner, float outer, float var) {
    float low = inner - (outer / 2.0f) * var;
    float high = inner + (outer / 2.0f) * var;
    float r = (float)rng.random();
    return low * (1 - r) + high * r;
}

ParticleSystem::ParticleSystem(Texture2D *texture, uint32 size)
    : pMem(nullptr), pFree(nullptr), pHead(nullptr), pTail(nullptr),
      texture(texture), active(true), insertMode(INSERT_MODE_TOP),
      maxParticles(0), activeParticles(0), emissionRate(0), emitCounter(0),
      emissionAreaDistribution(DISTRIBUTION_NONE), emissionAreaAngle(0),
      directionRelativeToEmissionCenter(false), lifetime(-1), life(0),
      particleLifeMin(0), particleLifeMax(0), direction(0), spread(0),
      speedMin(0), speedMax(0), linearAccelerationMax({0, 0}),
      linearAccelerationMin({0, 0}), radialAccelerationMax(0),
      radialAccelerationMin(0), tangentialAccelerationMax(0),
      tangentialAccelerationMin(0), linearDampingMax(0), linearDampingMin(0),
      sizeVariation(0), rotationMin(0), rotationMax(0), spinStart(0),
      spinEnd(0), spinVariation(0),
      offset({float(texture->width) * 0.5f, float(texture->height) * 0.5f}),
      defaultOffset(true), relativeRotation(false) {
    if (size == 0 || size > MAX_PARTICLES) {
        throw std::exception();
    }

    sizes.push_back(1.0f);
    colors.push_back({255, 255, 255, 255});
    setBufferSize(size);
}

ParticleSystem::ParticleSystem(const ParticleSystem &p)
    : pMem(nullptr), pFree(nullptr), pHead(nullptr), pTail(nullptr),
      texture(p.texture), active(p.active), insertMode(p.insertMode),
      maxParticles(p.maxParticles), activeParticles(0),
      emissionRate(p.emissionRate), emitCounter(0.0f), position(p.position),
      prevPosition(p.prevPosition),
      emissionAreaDistribution(p.emissionAreaDistribution),
      emissionArea(p.emissionArea), emissionAreaAngle(p.emissionAreaAngle),
      directionRelativeToEmissionCenter(p.directionRelativeToEmissionCenter),
      lifetime(p.lifetime),
      life(p.lifetime) // Initialize with the maximum life time.
      ,
      particleLifeMin(p.particleLifeMin), particleLifeMax(p.particleLifeMax),
      direction(p.direction), spread(p.spread), speedMin(p.speedMin),
      speedMax(p.speedMax), linearAccelerationMin(p.linearAccelerationMin),
      linearAccelerationMax(p.linearAccelerationMax),
      radialAccelerationMin(p.radialAccelerationMin),
      radialAccelerationMax(p.radialAccelerationMax),
      tangentialAccelerationMin(p.tangentialAccelerationMin),
      tangentialAccelerationMax(p.tangentialAccelerationMax),
      linearDampingMin(p.linearDampingMin),
      linearDampingMax(p.linearDampingMax), sizes(p.sizes),
      sizeVariation(p.sizeVariation), rotationMin(p.rotationMin),
      rotationMax(p.rotationMax), spinStart(p.spinStart), spinEnd(p.spinEnd),
      spinVariation(p.spinVariation), offset(p.offset),
      defaultOffset(p.defaultOffset), colors(p.colors), quads(p.quads),
      relativeRotation(p.relativeRotation) {
    setBufferSize(maxParticles);
}

ParticleSystem::~ParticleSystem() {
    deleteBuffers();
}

ParticleSystem *ParticleSystem::clone() {
    return new ParticleSystem(*this);
}

void ParticleSystem::resetOffset() {
    if (quads.empty())
        offset = {float(texture->width) * 0.5f, float(texture->height) * 0.5f};

    else {
        Quad::Viewport v = quads[0]->getViewport();
        offset = {static_cast<float>(v.w * 0.5f),
                  static_cast<float>(v.h * 0.5f)};
    }
}

void ParticleSystem::createBuffers(size_t size) {
    pFree = pMem = new Particle[size];
    maxParticles = (uint32)size;
}

void ParticleSystem::deleteBuffers() {
    delete[] pMem;
    pMem = nullptr;
    maxParticles = 0;
    activeParticles = 0;
}

void ParticleSystem::setBufferSize(uint32 size) {
    deleteBuffers();
    createBuffers(size);
    reset();
}

uint32 ParticleSystem::getBufferSize() const {
    return maxParticles;
}

void ParticleSystem::addParticle(float t) {
    if (isFull()) return;

    Particle *p = pFree++;

    initParticle(p, t);
    switch (insertMode) {
    default:
    case INSERT_MODE_TOP:
        insertTop(p);
        break;
    case INSERT_MODE_BOTTOM:
        insertBottom(p);
        break;
    case INSERT_MODE_RANDOM:
        insertRandom(p);
        break;
    }

    activeParticles++;
}

void ParticleSystem::initParticle(Particle *p, float t) {
    float min, max;

    Vector2 pos = Vector2Add(
        prevPosition, Vector2Scale(Vector2Subtract(position, prevPosition), t));
    p->origin = {pos.x, pos.y};
    min = particleLifeMin;
    max = particleLifeMax;

    if (min == max)
        p->life = min;
    else
        p->life = (float)rng.random(min, max);
    p->lifetime = p->life;
    p->position = pos;

    min = direction - spread / 2.0f;
    max = direction + spread / 2.0f;
    float dir = (float)rng.random(min, max);

    float rand_x, rand_y;
    float c, s;
    switch (emissionAreaDistribution) {
    case DISTRIBUTION_UNIFORM:
        c = cosf(emissionAreaAngle);
        s = sinf(emissionAreaAngle);
        rand_x = (float)rng.random(-emissionArea.x, emissionArea.x);
        rand_y = (float)rng.random(-emissionArea.y, emissionArea.y);
        p->position.x += rand_x * c - rand_y * s;
        p->position.y += rand_x * s + rand_y * c;
        break;
    case DISTRIBUTION_NORMAL:
        c = cosf(emissionAreaAngle);
        s = sinf(emissionAreaAngle);
        rand_x = (float)rng.randomNormal(emissionArea.x);
        rand_y = (float)rng.randomNormal(emissionArea.y);
        p->position.x += rand_x * c - rand_y * s;
        p->position.y += rand_x * s + rand_y * c;
        break;
    case DISTRIBUTION_ELLIPSE:
        c = cosf(emissionAreaAngle);
        s = sinf(emissionAreaAngle);
        rand_x = (float)rng.random(-1.0f, 1.0f);
        rand_y = (float)rng.random(-1.0f, 1.0f);
        min = emissionArea.x * (rand_x * sqrt(1 - 0.5f * pow(rand_y, 2)));
        max = emissionArea.y * (rand_y * sqrt(1 - 0.5f * pow(rand_x, 2)));
        p->position.x += min * c - max * s;
        p->position.y += min * s + max * c;
        break;
    case DISTRIBUTION_BORDER_ELLIPSE:
        c = cosf(emissionAreaAngle);
        s = sinf(emissionAreaAngle);
        rand_x = (float)rng.random(0, M_PI * 2);
        min = cosf(rand_x) * emissionArea.x;
        max = sinf(rand_x) * emissionArea.y;
        p->position.x += min * c - max * s;
        p->position.y += min * s + max * c;
        break;
    case DISTRIBUTION_BORDER_RECTANGLE:
        c = cosf(emissionAreaAngle);
        s = sinf(emissionAreaAngle);
        rand_x = (float)rng.random((emissionArea.x + emissionArea.y) * -2,
                                   (emissionArea.x + emissionArea.y) * 2);
        rand_y = emissionArea.y * 2;
        if (rand_x < -rand_y) {
            min = rand_x + rand_y + emissionArea.x;
            p->position.x += min * c - s * -emissionArea.y;
            p->position.y += min * s + c * -emissionArea.y;
        } else if (rand_x < 0) {
            max = rand_x + emissionArea.y;
            p->position.x += c * -emissionArea.x - max * s;
            p->position.y += s * -emissionArea.x + max * c;
        } else if (rand_x < rand_y) {
            max = rand_x - emissionArea.y;
            p->position.x += c * emissionArea.x - max * s;
            p->position.y += s * emissionArea.x + max * c;
        } else {
            min = rand_x - rand_y - emissionArea.x;
            p->position.x += min * c - s * emissionArea.y;
            p->position.y += min * s + c * emissionArea.y;
        }
    case DISTRIBUTION_NONE:
    default:
        break;
    }

    if (directionRelativeToEmissionCenter) {
        dir += atan2f(p->position.y - pos.y, p->position.x - pos.x);
    }

    min = speedMin;
    max = speedMax;

    float speed = (float)rng.random(min, max);
    p->velocity = {cosf(dir) * speed, sinf(dir) * speed};

    p->linearAcceleration.x =
        (float)rng.random(linearAccelerationMin.x, linearAccelerationMax.x);
    p->linearAcceleration.y =
        (float)rng.random(linearAccelerationMin.y, linearAccelerationMax.y);

    min = radialAccelerationMin;
    max = radialAccelerationMax;

    p->radialAcceleration = (float)rng.random(min, max);

    min = tangentialAccelerationMin;
    max = tangentialAccelerationMax;

    p->tangentialAcceleration = (float)rng.random(min, max);

    min = linearDampingMin;
    max = linearDampingMax;
    p->linearDamping = (float)rng.random(min, max);

    p->sizeOffset = (float)rng.random(sizeVariation);
    p->sizeIntervalSize =
        (1.0f - (float)rng.random(sizeVariation)) - p->sizeOffset;
    p->size = sizes[(size_t)(p->sizeOffset - 0.5f) * (sizes.size() - 1)];

    min = rotationMin;
    max = rotationMax;
    p->rotation = (float)rng.random(min, max);
    p->spinStart = calculate_variation(spinStart, spinEnd, spinVariation);
    p->spinEnd = calculate_variation(spinEnd, spinStart, spinVariation);

    p->angle = p->rotation;
    if (relativeRotation) p->angle += atan2f(p->velocity.y, p->velocity.x);

    p->color = colors[0];
    p->quadIndex = 0;
}

void ParticleSystem::insertTop(Particle *p) {
    if (pHead == nullptr) {
        pHead = p;
        p->prev = nullptr;
    } else {
        pTail->next = p;
        p->prev = pTail;
    }
    p->next = nullptr;
    pTail = p;
}

void ParticleSystem::insertBottom(Particle *p) {
    if (pTail == nullptr) {
        pTail = p;
        p->next = nullptr;
    } else {
        pHead->prev = p;
        p->next = pHead;
    }
    p->prev = nullptr;
    pHead = p;
}

void ParticleSystem::insertRandom(Particle *p) {
    uint64 pos = rng.rand() % ((int64)activeParticles + 1);

    if (pos == activeParticles) {
        Particle *pA = pHead;
        if (pA) pA->prev = p;
        p->prev = nullptr;
        p->next = pA;
        pHead = p;
        return;
    }

    Particle *pA = pMem + pos;
    Particle *pB = pA->next;
    pA->next = p;
    if (pB)
        pB->prev = p;
    else
        pTail = p;
    p->prev = pA;
    p->next = pB;
}

ParticleSystem::Particle *ParticleSystem::removeParticle(Particle *p) {
    Particle *pNext = nullptr;

    if (p->prev)
        p->prev->next = p->next;
    else
        pHead = p->next;

    if (p->next) {
        p->next->prev = p->prev;
        pNext = p->next;
    } else
        pTail = p->prev;

    pFree--;

    if (p != pFree) {
        *p = *pFree;
        if (pNext == pFree) pNext = p;

        if (p->prev)
            p->prev->next = p;
        else
            pHead = p;

        if (p->next)
            p->next->prev = p;
        else
            pTail = p;
    }

    activeParticles--;
    return pNext;
}

void ParticleSystem::setTexture(std::shared_ptr<Texture2D> texture) {
    this->texture = texture;
    if (defaultOffset) resetOffset();
}

std::shared_ptr<Texture2D> ParticleSystem::getTexture() const {
    return texture;
}

void ParticleSystem::setInsertMode(InsertMode mode) {
    insertMode = mode;
}

ParticleSystem::InsertMode ParticleSystem::getInsertMode() const {
    return insertMode;
}

void ParticleSystem::setEmissionRate(float rate) {
    emissionRate = rate;
    emitCounter = std::min(emitCounter, 1.0f / rate);
}

float ParticleSystem::getEmissionRate() const {
    return emissionRate;
}

void ParticleSystem::setEmitterLifetime(float life) {
    this->life = lifetime = life;
}

float ParticleSystem::getEmitterLifetime() const {
    return lifetime;
}

void ParticleSystem::setParticleLifetime(float min, float max) {
    particleLifeMin = min;
    if (max == 0) {
        particleLifeMax = min;
    } else {
        particleLifeMax = max;
    }
}

void ParticleSystem::getParticleLifetime(float &min, float &max) const {
    min = particleLifeMin;
    max = particleLifeMax;
}

void ParticleSystem::setPosition(float x, float y) {
    position = {x, y};
    prevPosition = position;
}

const Vector2 &ParticleSystem::getPosition() const {
    return position;
}

void ParticleSystem::moveTo(float x, float y) {
    position = {x, y};
}

void ParticleSystem::setEmissionArea(AreaSpreadDistribution distribution,
                                     float x, float y, float angle,
                                     bool directionRelativeToCenter) {
    emissionAreaDistribution = distribution;
    emissionArea = {x, y};
    emissionAreaAngle = angle;
    this->directionRelativeToEmissionCenter = directionRelativeToCenter;
}

ParticleSystem::AreaSpreadDistribution
ParticleSystem::getEmissionArea(Vector2 &params, float &angle,
                                bool &directionRelativeToCenter) const {
    params = emissionArea;
    angle = emissionAreaAngle;
    directionRelativeToCenter = directionRelativeToEmissionCenter;
    return emissionAreaDistribution;
}

void ParticleSystem::setDirection(float direction) {
    this->direction = direction;
}

float ParticleSystem::getDirection() const {
    return direction;
}

void ParticleSystem::setSpread(float spread) {
    this->spread = spread;
}

float ParticleSystem::getSpread() const {
    return spread;
}

void ParticleSystem::setSpeed(float speed) {
    speedMin = speedMax = speed;
}

void ParticleSystem::setSpeed(float min, float max) {
    speedMin = min;
    speedMax = max;
}

void ParticleSystem::getSpeed(float &min, float &max) const {
    min = speedMin;
    max = speedMax;
}

void ParticleSystem::setLinearAcceleration(float x, float y) {
    linearAccelerationMin = linearAccelerationMax = {x, y};
}

void ParticleSystem::setLinearAcceleration(float xmin, float ymin, float xmax,
                                           float ymax) {
    linearAccelerationMin = {xmin, ymin};
    linearAccelerationMax = {xmax, ymax};
}

void ParticleSystem::getLinearAcceleration(Vector2 &min, Vector2 &max) const {
    min = linearAccelerationMin;
    max = linearAccelerationMax;
}

void ParticleSystem::setRadialAcceleration(float acceleration) {
    radialAccelerationMin = radialAccelerationMax = acceleration;
}

void ParticleSystem::setRadialAcceleration(float min, float max) {
    radialAccelerationMin = min;
    radialAccelerationMax = max;
}

void ParticleSystem::getRadialAcceleration(float &min, float &max) const {
    min = radialAccelerationMin;
    max = radialAccelerationMax;
}

void ParticleSystem::setTangentialAcceleration(float acceleration) {
    tangentialAccelerationMin = tangentialAccelerationMax = acceleration;
}

void ParticleSystem::setTangentialAcceleration(float min, float max) {
    tangentialAccelerationMin = min;
    tangentialAccelerationMax = max;
}

void ParticleSystem::getTangentialAcceleration(float &min, float &max) const {
    min = tangentialAccelerationMin;
    max = tangentialAccelerationMax;
}

void ParticleSystem::setLinearDamping(float min, float max) {
    linearDampingMin = min;
    linearDampingMax = max;
}

void ParticleSystem::getLinearDamping(float &min, float &max) const {
    min = linearDampingMin;
    max = linearDampingMax;
}

void ParticleSystem::setSize(float size) {
    sizes.resize(1);
    sizes[0] = size;
}

void ParticleSystem::setSizes(const std::vector<float> &newSizes) {
    sizes = newSizes;
}

const std::vector<float> &ParticleSystem::getSizes() const {
    return sizes;
}

void ParticleSystem::setSizeVariation(float variation) {
    sizeVariation = variation;
}

float ParticleSystem::getSizeVariation() const {
    return sizeVariation;
}

void ParticleSystem::setRotation(float rotation) {
    rotationMin = rotationMax = rotation;
}

void ParticleSystem::setRotation(float min, float max) {
    rotationMin = min;
    rotationMax = max;
}

void ParticleSystem::getRotation(float &min, float &max) const {
    min = rotationMin;
    max = rotationMax;
}

void ParticleSystem::setSpin(float start, float end) {
    spinStart = start;
    spinEnd = end;
}

void ParticleSystem::getSpin(float &start, float &end) const {
    start = spinStart;
    end = spinEnd;
}

void ParticleSystem::setSpin(float spin) {
    spinStart = spinEnd = spin;
}

void ParticleSystem::setSpinVariation(float variation) {
    spinVariation = variation;
}

float ParticleSystem::getSpinVariation() const {
    return spinVariation;
}

void ParticleSystem::setOffset(float x, float y) {
    offset = {x, y};
    defaultOffset = false;
}

Vector2 ParticleSystem::getOffset() const {
    return offset;
}

void ParticleSystem::setColor(const std::vector<Color> &newColors) {
    colors = newColors;
}

std::vector<Color> ParticleSystem::getColors() const {
    return colors;
}

void ParticleSystem::setQuads(const std::vector<Quad *> &newQuads) {
    std::vector<std::shared_ptr<Quad>> quadList;
    for (Quad *q : newQuads) {
        quadList.push_back(std::shared_ptr<Quad>(q));
    }
    quads = quadList;
    if (defaultOffset) resetOffset();
}

void ParticleSystem::setQuads() {
    quads.clear();
}

std::vector<Quad *> ParticleSystem::getQuads() const {
    std::vector<Quad *> quadList;
    for (std::shared_ptr<Quad> q : quads) {
        quadList.push_back(q.get());
    }
    return quadList;
}

void ParticleSystem::setRelativeRotation(bool enable) {
    relativeRotation = enable;
}

bool ParticleSystem::hasRelativeRotation() const {
    return relativeRotation;
}

uint32 ParticleSystem::getCount() const {
    return activeParticles;
}

void ParticleSystem::start() {
    active = true;
}

void ParticleSystem::stop() {
    active = false;
    life = lifetime;
    emitCounter = 0;
}

void ParticleSystem::pause() {
    active = false;
}

void ParticleSystem::reset() {
    if (pMem == nullptr) return;
    life = lifetime;
    emitCounter = 0;
    activeParticles = 0;
    pHead = pTail = nullptr;
    pFree = pMem;
}

void ParticleSystem::emit(uint32 num) {
    if (!active) return;

    num = std::min(num, maxParticles - activeParticles);
    while (num--) addParticle(1.0f);
}

bool ParticleSystem::isActive() const {
    return active;
}

bool ParticleSystem::isPaused() const {
    return !active && life < lifetime;
}

bool ParticleSystem::isStopped() const {
    return !active && life >= lifetime;
}

bool ParticleSystem::isEmpty() const {
    return activeParticles == 0;
}

bool ParticleSystem::isFull() const {
    return activeParticles == maxParticles;
}

void ParticleSystem::update(float dt) {
    if (pMem == nullptr || dt == 0.0f) return;

    Particle *p = pHead;

    while (p) {
        p->life -= dt;

        if (p->life <= 0) {
            p = removeParticle(p);
        } else {
            Vector2 radial, tangential;
            Vector2 ppos = p->position;
            radial = {ppos.x - p->origin.x, ppos.y - p->origin.y};
            radial = Vector2Normalize(radial);
            tangential = radial;

            radial = Vector2Scale(radial, p->radialAcceleration);

            {
                float a = tangential.x;
                tangential.x = -tangential.y;
                tangential.y = a;
            }

            tangential = Vector2Scale(tangential, p->tangentialAcceleration);
            p->velocity = Vector2Add(
                p->velocity,
                Vector2Scale(Vector2Add(Vector2Add(radial, tangential),
                                        p->linearAcceleration),
                             dt));
            p->velocity = Vector2Scale(p->velocity,
                                       1.0f / (1.0f + p->linearDamping * dt));
            ppos = Vector2Add(ppos, Vector2Scale(p->velocity, dt));
            p->position = ppos;

            const float t = 1.0f - p->life / p->lifetime;

            p->rotation += (p->spinStart * (1.0f - t) + p->spinEnd * t) * dt;
            p->angle = p->rotation;

            if (relativeRotation) {
                p->angle += atan2f(p->velocity.y, p->velocity.x);
            }

            // Change size according to given intervals:
            // i = 0       1       2      3          n-1
            //     |-------|-------|------|--- ... ---|
            // t = 0    1/(n-1)        3/(n-1)        1
            //
            // `s' is the interpolation variable scaled to the current
            // interval width, e.g. if n = 5 and t = 0.3, then the current
            // indices are 1,2 and s = 0.3 - 0.25 = 0.05
            //

            float s = p->sizeOffset + t * p->sizeIntervalSize;
            s *= (float)(sizes.size() - 1);
            size_t i = (size_t)s;
            size_t k = (i == sizes.size() - 1) ? i : i + 1;
            s -= (float)i;
            p->size = sizes[i] * (1.0f - s) + sizes[k] * s;

            s = t * (float)(colors.size() - 1);
            i = (size_t)s;
            k = (i == colors.size() - 1) ? i : i + 1;
            s -= (float)i;
            p->color = {(uint8)(colors[i].r * (1.0f - s) + colors[k].r * s),
                        (uint8)(colors[i].g * (1.0f - s) + colors[k].g * s),
                        (uint8)(colors[i].b * (1.0f - s) + colors[k].b * s),
                        (uint8)(colors[i].a * (1.0f - s) + colors[k].a * s)};

            k = quads.size();
            if (k > 0) {
                s = t * (float)(k);
                i = (s > 0.0f) ? (size_t)s : 0;
                p->quadIndex = (int)((i < k) ? i : k - 1);
            }

            p = p->next;
        }
    }

    if (active) {
        float rate = 1.0f / emissionRate;
        emitCounter += dt;
        float total = emitCounter - rate;
        while (emitCounter > rate) {
            addParticle(1.0f - (emitCounter - rate) / total);
            emitCounter -= rate;
        }

        life -= dt;
        if (lifetime != -1 && life < 0) stop();
    }

    prevPosition = position;
    // print active particles
    std::cout << "Active particles: " << activeParticles << std::endl;
}

void ParticleSystem::draw() {
    uint32 pCount = getCount();

    if (pCount == 0 || pMem == nullptr || texture == nullptr) return;

    Particle *p = pHead;

    bool useQuads = !quads.empty();

    while (p) {

        float rotation = p->angle * RAD2DEG;
        Vector2 origin = {offset.x, offset.y};
        Vector2 scale = {p->size, p->size};

        if (useQuads) {
            Quad *q = quads[p->quadIndex].get();
            Quad::Viewport v = q->getViewport();
            Rectangle sourceRec = {
                static_cast<float>(v.x), static_cast<float>(v.y),
                static_cast<float>(v.w), static_cast<float>(v.h)};
            Rectangle destRec = {p->position.x, p->position.y,
                                 sourceRec.width * scale.x,
                                 sourceRec.height * scale.y};
            DrawTexturePro(*texture, sourceRec, destRec, origin, rotation,
                           p->color);
        } else {
            Rectangle destRec = {p->position.x - texture->width * scale.x / 2,
                                 p->position.y - texture->height * scale.y / 2,
                                 texture->width * scale.x,
                                 texture->height * scale.y};
            Rectangle sourceRec = {0, 0, static_cast<float>(texture->width),
                                   static_cast<float>(texture->height)};
            DrawTexturePro(*texture, sourceRec, destRec, origin, rotation,
                           p->color);
        }

        p = p->next;

        // need to check quadIndex and draw quads otherwise draw texture
    }
}

// bool ParticleSystem::getConstant(const char *in, AreaSpreadDistribution &out)
// {
//     return distributions.find(in, out);
// }
//
// bool ParticleSystem::getConstant(AreaSpreadDistribution in, const char *&out)
// {
//     return distributions.find(in, out);
// }
//
// std::vector<std::string> ParticleSystem::getConstants(AreaSpreadDistribution)
// {
//     return distributions.getNames();
// }
//
// bool ParticleSystem::getConstant(const char *in, InsertMode &out) {
//     return insertModes.find(in, out);
// }
//
// bool ParticleSystem::getConstant(InsertMode in, const char *&out) {
//     return insertModes.find(in, out);
// }
//
// std::vector<std::string> ParticleSystem::getConstants(InsertMode) {
//     return insertModes.getNames();
// }
//
// StringMap<ParticleSystem::AreaSpreadDistribution,
//           ParticleSystem::DISTRIBUTION_MAX_ENUM>::Entry
//     ParticleSystem::distributionsEntries[] = {
//         {"none", DISTRIBUTION_NONE},
//         {"uniform", DISTRIBUTION_UNIFORM},
//         {"normal", DISTRIBUTION_NORMAL},
//         {"ellipse", DISTRIBUTION_ELLIPSE},
//         {"borderellipse", DISTRIBUTION_BORDER_ELLIPSE},
//         {"borderrectangle", DISTRIBUTION_BORDER_RECTANGLE}};
//
// StringMap<ParticleSystem::InsertMode,
//           ParticleSystem::INSERT_MODE_MAX_ENUM>::Entry
//     ParticleSystem::insertModesEntries[] = {{"top", INSERT_MODE_TOP},
//                                             {"bottom", INSERT_MODE_BOTTOM},
//                                             {"random", INSERT_MODE_RANDOM}};
//
// StringMap<ParticleSystem::AreaSpreadDistribution,
//           ParticleSystem::DISTRIBUTION_MAX_ENUM>
//     ParticleSystem::distributions(ParticleSystem::distributionsEntries,
//                                   sizeof(ParticleSystem::distributionsEntries));
//
// StringMap<ParticleSystem::InsertMode, ParticleSystem::INSERT_MODE_MAX_ENUM>
//     ParticleSystem::insertModes(ParticleSystem::insertModesEntries,
//                                 sizeof(ParticleSystem::insertModesEntries));
