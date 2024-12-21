#pragma once
#include "jni.h"
#include <string>
#include <map>

void sendFirebaseEvent(const std::string &eventName,
                       const std::map<std::string, std::string> &eventData);
void sendFirebaseEventAndroid(
    const std::string &eventName,
    const std::map<std::string, std::string> &eventData);
void sendFirebaseWebEvent(const std::string &eventName,
                          const std::map<std::string, std::string> &eventData);
void sendFirebaseDesktopEvent(
    const std::string &eventName,
    const std::map<std::string, std::string> &eventData);

void setJavaVM(JavaVM *vm);
void setActivity(jobject activityRef);
void clearActivity(JNIEnv *env);
