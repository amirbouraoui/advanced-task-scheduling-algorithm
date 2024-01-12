#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

#include "application_Scheduling.h"
#include "algo.h"
#include "util.h"
#include "list.h"
#include "tree.h"
#include "instance.h"
#include "schedule.h"

JNIEXPORT jint JNICALL Java_application_Scheduling_runSchedule(JNIEnv *env, jobject obj, jstring instanceInFile, jstring scheduleOutFile, jint m, jint preemption, jint balanced)
{
  // Suppress unused parameter 'obj'
  (void)obj;

  // Convert Java Strings to C strings
  const char *instanceInFileStr = (*env)->GetStringUTFChars(env, instanceInFile, NULL);
  if (instanceInFileStr == NULL)
  {
    return -1; /* OutOfMemoryError already thrown */
  }

  const char *scheduleOutFileStr = (*env)->GetStringUTFChars(env, scheduleOutFile, NULL);
  if (scheduleOutFileStr == NULL)
  {
    (*env)->ReleaseStringUTFChars(env, instanceInFile, instanceInFileStr);
    return -1; /* OutOfMemoryError already thrown */
  }

  // Read the instance and create the schedule
  Instance I = read_instance(instanceInFileStr);
  struct schedule_t *S = create_schedule(I, m, preemption, balanced);

  // Save the schedule and delete it along with the instance
  save_schedule(S, scheduleOutFileStr);
  delete_schedule(S);
  delete_instance(I, 1);

  // Release the C strings
  (*env)->ReleaseStringUTFChars(env, instanceInFile, instanceInFileStr);
  (*env)->ReleaseStringUTFChars(env, scheduleOutFile, scheduleOutFileStr);

  // Indicate success
  return 0;
}
