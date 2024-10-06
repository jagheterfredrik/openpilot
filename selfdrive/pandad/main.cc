#include <cassert>

#include <jni.h>
#include <android/file_descriptor_jni.h>
// #include <stdio.h>

// #include <android/log.h>

// #define DEBUG_TAG "pandad"
// #define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,DEBUG_TAG,__VA_ARGS__)
// #define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,DEBUG_TAG,__VA_ARGS__)
// #define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,DEBUG_TAG,__VA_ARGS__)

#include "selfdrive/pandad/pandad.h"
#include "common/swaglog.h"
#include "common/util.h"
#include "system/hardware/hw.h"

int main(int argc, char *argv[]) {
  LOGW("starting pandad");

  if (!Hardware::PC()) {
    int err;
    err = util::set_realtime_priority(54);
    assert(err == 0);
    err = util::set_core_affinity({3});
    assert(err == 0);
  }

  std::vector<std::string> serials(argv + 1, argv + argc);
  pandad_main_thread(serials);
  return 0;
}

#define JNIEXPORT  __attribute__ ((visibility ("default")))
extern "C" {

JNIEXPORT void Java_ai_flow_flowy_ServicePandad_nativeInit(JNIEnv* env, jclass cls, jint fd) {
  
  // int fd = AFileDescriptor_getFd(env, fileDescriptor);
  // FILE * f = fopen("/data/data/org.jagheterfredrik.flowapp/files/fflog", "a");
  // fprintf(f, "STARTING PANDA -!? %d\n", fd);
  // fclose(f);
  char zmq[] = "ZMQ=1";
  putenv(zmq);
  pandad_main_thread(fd);
}

}