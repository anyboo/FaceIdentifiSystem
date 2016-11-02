#ifndef DEF_H
#define DEF_H
#include <functional>
#include <windef.h>

#define DUTY_ERROR_NOERROR        0
#define DUTY_ERROR_NOBODY         1
#define DUTY_ERROR_NONE_ON_DUTY   2
#define DUTY_ERROR_MAN_OFF_WORK   3

#define ERROR_CONTAIN_TIME_SECONDS   10

#define SIMILITY_PASS  70
#define VERIFY_SLEEP_TIME_MILLI_SECONDS   500
#define MAX_VERIFITY_SECONDS   15
#define MAX_NO_FRAME_SECONDS   15
typedef std::function<void(const char*, UINT, int, int, UINT)> ThreadCallback;
#endif // DEF_H
