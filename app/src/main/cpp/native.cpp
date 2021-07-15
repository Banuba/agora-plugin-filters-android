#include <jni.h>
#include <string>
#include <filter/BanubaExtensionProvider.h>

std::string jstring2string(JNIEnv *env, jstring jstr) {
    const char *chars = env->GetStringUTFChars(jstr, nullptr);
    std::string ret(chars);
    env->ReleaseStringUTFChars(jstr, chars);
    return ret;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_banuba_sdk_agorapluginexample_ExtensionManager_nativeGetExtensionProvider(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jstring jVendor,
                                                                                   jstring resources_path,
                                                                                   jstring token,
                                                                                   jint width,
                                                                                   jint height) {
    std::vector<std::string> paths{jstring2string(env, resources_path)};
    auto client_token = jstring2string(env, token);
    agora::extension::BanubaExtensionProvider::create(paths, client_token, width, height);
    agora::extension::BanubaExtensionProvider *provider = agora::extension::BanubaExtensionProvider::getInstance();
    const char *vendor = env->GetStringUTFChars(jVendor, nullptr);
    provider->setExtensionVendor(vendor);
    return reinterpret_cast<intptr_t>(provider);
}