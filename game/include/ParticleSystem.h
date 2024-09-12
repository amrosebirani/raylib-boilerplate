#pragma once
#include "int.h"
#include "Quad.h"
#include "raylib.h"
#include <vector>
#include <string>
#include <memory>
// #include "StringMap.h"
#define INT32_MAX_X 0x7FFFFFFF

class ParticleSystem {
    public:
        enum AreaSpreadDistribution {
            DISTRIBUTION_NONE,
            DISTRIBUTION_UNIFORM,
            DISTRIBUTION_NORMAL,
            DISTRIBUTION_ELLIPSE,
            DISTRIBUTION_BORDER_ELLIPSE,
            DISTRIBUTION_BORDER_RECTANGLE,
            DISTRIBUTION_MAX_ENUM
        };

        enum InsertMode {
            INSERT_MODE_TOP,
            INSERT_MODE_BOTTOM,
            INSERT_MODE_RANDOM,
            INSERT_MODE_MAX_ENUM
        };

        static const uint32 MAX_PARTICLES = INT32_MAX_X / 4;

        ParticleSystem(Texture2D *texture, uint32 buffer);
        ParticleSystem(const ParticleSystem &other);

        virtual ~ParticleSystem();

        ParticleSystem *clone();
        void setTexture(std::shared_ptr<Texture2D> texture);
        std::shared_ptr<Texture2D> getTexture() const;

        void setBufferSize(uint32 size);
        uint32 getBufferSize() const;

        void setInsertMode(InsertMode mode);
        InsertMode getInsertMode() const;

        void setEmissionRate(float rate);
        float getEmissionRate() const;

        void setEmitterLifetime(float lifetime);
        float getEmitterLifetime() const;

        void setParticleLifetime(float min, float max = 0);

        void getParticleLifetime(float &min, float &max) const;

        void setPosition(float x, float y);
        const Vector2 &getPosition() const;

        void moveTo(float x, float y);

        void setEmissionArea(AreaSpreadDistribution distribution, float x,
                             float y, float angle,
                             bool directionRelativeToCenter);
        AreaSpreadDistribution
        getEmissionArea(Vector2 &params, float &angle,
                        bool &directionRelativeToCenter) const;

        void setDirection(float direction);
        float getDirection() const;

        void setSpread(float spread);
        float getSpread() const;

        void setSpeed(float speed);
        void setSpeed(float min, float max);

        void getSpeed(float &min, float &max) const;

        void setLinearAcceleration(float x, float y);
        void setLinearAcceleration(float xmin, float ymin, float xmax,
                                   float ymax);
        void getLinearAcceleration(Vector2 &min, Vector2 &max) const;
        void setRadialAcceleration(float acceleration);
        void setRadialAcceleration(float min, float max);
        void getRadialAcceleration(float &min, float &max) const;
        void setTangentialAcceleration(float acceleration);
        void setTangentialAcceleration(float min, float max);
        void getTangentialAcceleration(float &min, float &max) const;
        void setLinearDamping(float min, float max);
        void getLinearDamping(float &min, float &max) const;
        void setSize(float size);
        void setSizes(const std::vector<float> &newSizes);
        const std::vector<float> &getSizes() const;
        void setSizeVariation(float variation);
        float getSizeVariation() const;
        void setRotation(float rotation);
        void setRotation(float min, float max);
        void getRotation(float &min, float &max) const;
        void setSpin(float spin);
        void setSpin(float start, float end);
        void getSpin(float &start, float &end) const;
        void setSpinVariation(float variation);
        float getSpinVariation() const;
        void setOffset(float x, float y);
        Vector2 getOffset() const;

        void setColor(const std::vector<Color> &newColors);
        std::vector<Color> getColors() const;

        void setQuads(const std::vector<Quad *> &newQuads);
        void setQuads();

        std::vector<Quad *> getQuads() const;

        void setRelativeRotation(bool enable);
        bool hasRelativeRotation() const;

        uint32 getCount() const;

        void start();
        void stop();
        void pause();
        void reset();

        void emit(uint32 count);
        void justEmit(uint32 count);

        bool isActive() const;

        bool isPaused() const;
        bool isStopped() const;
        bool isEmpty() const;
        bool isFull() const;

        void update(float delta);

        void draw();

        static bool getConstant(const char *in, AreaSpreadDistribution &out);
        static bool getConstant(AreaSpreadDistribution in, const char *&out);
        static std::vector<std::string> getConstants(AreaSpreadDistribution);

        static bool getConstant(const char *in, InsertMode &out);
        static bool getConstant(InsertMode in, const char *&out);
        static std::vector<std::string> getConstants(InsertMode);

    private:
        struct Particle {
                Particle *prev;
                Particle *next;

                float lifetime;
                float life;
                Vector2 position;
                Vector2 origin;
                Vector2 velocity;
                Vector2 linearAcceleration;
                float radialAcceleration;
                float tangentialAcceleration;

                float linearDamping;

                float size;
                float sizeOffset;
                float sizeIntervalSize;

                float rotation;
                float angle;
                float spinStart;
                float spinEnd;

                Color color;
                int quadIndex;
        };

        void resetOffset();

        void createBuffers(size_t size);
        void deleteBuffers();

        void addParticle(float t);
        Particle *removeParticle(Particle *p);

        void initParticle(Particle *p, float t);
        void insertTop(Particle *p);
        void insertBottom(Particle *p);
        void insertRandom(Particle *p);

        Particle *pMem;

        Particle *pFree;

        Particle *pHead;

        Particle *pTail;

        std::shared_ptr<Texture2D> texture;

        bool active;

        InsertMode insertMode;

        uint32 maxParticles;

        uint32 activeParticles;

        float emissionRate;

        float emitCounter;

        Vector2 position;
        Vector2 prevPosition;

        AreaSpreadDistribution emissionAreaDistribution;
        Vector2 emissionArea;

        float emissionAreaAngle;
        bool directionRelativeToEmissionCenter;

        float lifetime;
        float life;

        float direction;
        float spread;

        float particleLifeMin;
        float particleLifeMax;

        float speedMin;
        float speedMax;

        Vector2 linearAccelerationMin;
        Vector2 linearAccelerationMax;

        float radialAccelerationMin;
        float radialAccelerationMax;

        float tangentialAccelerationMin;
        float tangentialAccelerationMax;

        float linearDampingMin;
        float linearDampingMax;

        std::vector<float> sizes;
        float sizeVariation;

        float rotationMin;
        float rotationMax;

        float spinStart;
        float spinEnd;
        float spinVariation;

        Vector2 offset;

        bool defaultOffset;

        std::vector<Color> colors;
        std::vector<std::shared_ptr<Quad>> quads;

        bool relativeRotation;
        // static StringMap<AreaSpreadDistribution,
        // DISTRIBUTION_MAX_ENUM>::Entry distributionsEntries[];
        // static StringMap<AreaSpreadDistribution, DISTRIBUTION_MAX_ENUM>
        //     distributions;
        //
        // static StringMap<InsertMode, INSERT_MODE_MAX_ENUM>::Entry
        //     insertModesEntries[];
        // static StringMap<InsertMode, INSERT_MODE_MAX_ENUM> insertModes;
};
