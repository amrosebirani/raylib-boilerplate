#include "firebase.hpp"
#include "platform.hpp"
#include "session.hpp"
#if defined(PLATFORM_DESKTOP)
#include "curl/curl.h"
#endif
#include <sstream>
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif
#include <iostream>

static JavaVM *javaVM = nullptr;
static jobject activity = nullptr;

const std::string FIREBASE_MEASUREMENT_ID = "G-CG3WVPFRES";
const std::string FIREBASE_API_SECRET = "8XcZ_REjTLmRt1-FqYrXhA";
std::string firebase_url =
    "https://www.google-analytics.com/mp/collect?measurement_id=" +
    FIREBASE_MEASUREMENT_ID + "&api_secret=" + FIREBASE_API_SECRET;

void setJavaVM(JavaVM *vm) {
    javaVM = vm;
}

void setActivity(jobject activityRef) {
    activity = activityRef;
}

void clearActivity(JNIEnv *env) {
    if (activity) {
        env->DeleteGlobalRef(activity);
        activity = nullptr;
    }
}

std::string MapToJson(const std::map<std::string, std::string> &keyValueMap) {
    std::ostringstream oss;
    oss << "{";
    for (auto it = keyValueMap.begin(); it != keyValueMap.end(); ++it) {
        oss << "\"" << it->first << "\": \"" << it->second << "\"";
        if (std::next(it) != keyValueMap.end()) {
            oss << ",";
        }
    }
    oss << "}";
    return oss.str();
}

void sendFirebaseEvent(const std::string &eventName,
                       const std::map<std::string, std::string> &eventData) {
    if (isPlatformAndroid()) {
        sendFirebaseEventAndroid(eventName, eventData);
    }
    if (isPlatformWeb()) {
        sendFirebaseWebEvent(eventName, eventData);
    }
    if (isPlatformDesktop()) {
        sendFirebaseDesktopEvent(eventName, eventData);
    }
}

void sendFirebaseDesktopEvent(
    const std::string &eventName,
    const std::map<std::string, std::string> &keyValueMap) {
#if defined(PLATFORM_DESKTOP)
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize cURL" << std::endl;
        return;
    }
    std::string jsonPayload = R"({
        "client_id": ")" + getSessionId() +
                              R"(",
        "events": [
            {
                "name": ")" + eventName +
                              R"(",
                "params":)" + MapToJson(keyValueMap) +
                              R"( 
            }
        ]
    })";
    curl_easy_setopt(curl, CURLOPT_URL, firebase_url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER,
                     curl_slist_append(NULL, "Content-Type: application/json"));

    // Perform the request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "Event sent successfully to Firebase" << std::endl;
    }
    curl_easy_cleanup(curl);

#endif
}

void sendFirebaseWebEvent(
    const std::string &eventName,
    const std::map<std::string, std::string> &eventParams) {
#if defined(PLATFORM_WEB)
    std::string jsonParams = "{";

    jsonParams += "\"session_id\": \"" + getSessionId() + "\"";
    for (auto it = eventParams.begin(); it != eventParams.end(); ++it) {
        jsonParams += ",";
        jsonParams += "\"" + it->first + "\": \"" + it->second + "\"";
        // if (std::next(it) != eventParams.end()) {
        //     jsonParams += ",";
        // }
    }
    jsonParams += "}";

    // Call JavaScript function via EM_ASM
    EM_ASM(
        {
            const eventName = UTF8ToString($0);
            const eventParams = JSON.parse(UTF8ToString($1));
            logFirebaseEvent(eventName, eventParams);
        },
        eventName.c_str(), jsonParams.c_str());
#endif
}

void sendFirebaseEventAndroid(
    const std::string &eventName,
    const std::map<std::string, std::string> &eventData) {
    JNIEnv *env = nullptr;
    if (!javaVM) {
        // TraceLog(LOG_ERROR, "JavaVM is null");
        return;
    }
    jint result = javaVM->AttachCurrentThread(&env, nullptr);
    if (result != JNI_OK || !env) {
        // TraceLog(LOG_ERROR, "Failed to attach thread to JavaVM");
        return;
    }
    // Get the global JNI environment for Android
    if (!activity) {
        // TraceLog(LOG_ERROR, "Activity is null");
        return;
    }
    jstring jEventName = env->NewStringUTF(eventName.c_str());

    // Convert std::map to Java HashMap
    jclass hashMapClass = env->FindClass("java/util/HashMap");
    jmethodID hashMapInit = env->GetMethodID(hashMapClass, "<init>", "()V");
    jobject hashMap = env->NewObject(hashMapClass, hashMapInit);

    jmethodID hashMapPut = env->GetMethodID(
        hashMapClass, "put",
        "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    for (const auto &pair : eventData) {
        jstring key = env->NewStringUTF(pair.first.c_str());
        jstring value = env->NewStringUTF(pair.second.c_str());
        env->CallObjectMethod(hashMap, hashMapPut, key, value);
        env->DeleteLocalRef(key);
        env->DeleteLocalRef(value);
    }
    jstring key = env->NewStringUTF("session_id");
    jstring value = env->NewStringUTF(getSessionId().c_str());
    env->CallObjectMethod(hashMap, hashMapPut, key, value);
    env->DeleteLocalRef(key);
    env->DeleteLocalRef(value);
    jclass activityClass = env->GetObjectClass(activity);
    jmethodID sendEventMethod =
        env->GetMethodID(activityClass, "sendFirebaseEvent",
                         "(Ljava/lang/String;Ljava/util/Map;)V");

    env->CallVoidMethod(activity, sendEventMethod, jEventName, hashMap);

    // Clean up references
    env->DeleteLocalRef(jEventName);
    env->DeleteLocalRef(hashMap);
    env->DeleteLocalRef(activityClass);

    // Detach from the thread if necessary
    javaVM->DetachCurrentThread();
}
