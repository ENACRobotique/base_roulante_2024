#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ch.h"
#include "hal.h"
#include "microrl/microrlShell.h"
#include "ttyConsole.h"
#include "stdutil.h"
#include "usb_serial.h"
//#include "rtcAccess.h"
#include "printf.h"
#include "globalVar.h"
#include "motor.h"


/*===========================================================================*/
/* START OF EDITABLE SECTION                                           */
/*===========================================================================*/


static void cmd_mem(BaseSequentialStream *lchp, int argc,const char * const argv[]);
static void cmd_threads(BaseSequentialStream *lchp, int argc,const char * const argv[]);
//static void cmd_rtc(BaseSequentialStream *lchp, int argc,const char * const argv[]);
static void cmd_uid(BaseSequentialStream *lchp, int argc,const char * const argv[]);
static void cmd_help(BaseSequentialStream *lchp, int argc,const char * const argv[]);
static void cmd_mot(BaseSequentialStream *lchp, int argc,const char * const argv[]);
static void cmd_encf(BaseSequentialStream *lchp, int argc,const char * const argv[]);
static void cmd_odom(BaseSequentialStream *lchp, int argc,const char * const argv[]);
static void cmd_guidance(BaseSequentialStream *lchp, int argc,const char * const argv[]);



static const ShellCommand commands[] = {
  
  {"mem", cmd_mem},
  {"threads", cmd_threads},
  //{"rtc", cmd_rtc},
  {"uid", cmd_uid},
  {"help", cmd_help},
  {"mot", cmd_mot},
  {"encf", cmd_encf},
  {"odom", cmd_odom},
  {"goto", cmd_guidance},
  {NULL, NULL}
};




static void cmd_help(BaseSequentialStream *lchp, int argc,const char * const argv[]) {
  if(argc == 0) {
    chprintf (lchp, "Commands available:\r\n");
    chprintf (lchp, "\tmem:\r\n");
    chprintf (lchp, "\tthreads: info about threads\n");
    chprintf (lchp, "\tuid: get chip unique ID\r\n");
    chprintf (lchp, "\thelp: get help\r\n");
    chprintf (lchp, "\tmot:\r\n");
    chprintf (lchp, "\tenc: get encoder value\r\n");
  } else if(memcmp(argv[0], "mot", strlen(argv[0])) == 0) {
    chprintf (lchp, "Usage: mot <mot_no> <speed>\r\n");
    chprintf (lchp, "mot_no in [1 .. 3], speed in [-100.0 .. 100.0]\r\n");
  } else if(memcmp(argv[0], "enc", strlen(argv[0])) == 0) {
    chprintf (lchp, "Usage: enc <enc_no>\r\n");
    chprintf (lchp, "enc_no in [1 .. 3]\r\n");
  } else {
    chprintf (lchp, "No help for \"%s\"\r\n", argv[0]);
  }
}

static void cmd_mot(BaseSequentialStream *lchp, int argc,const char * const argv[]) {
  if (argc < 2){
    chprintf (lchp, "Usage: mot <n> <speed>\r\n");
    return;
  }
  int motor_id = atoi(argv[0]);
  if(motor_id >= 0 && motor_id <= MOTORS_NB) {
    motors[motor_id].set_cmd(atoi(argv[1]));
  }
}


static void cmd_encf(BaseSequentialStream *lchp, int argc,const char * const argv[]) {
  (void)argv;
  (void)argc;
  if(argc < 1) {
    chprintf (lchp, "Usage: encf <n>\r\n");
    return;
  }
  int motor_id = atoi(argv[0]);
  if(motor_id >= 0 && motor_id <= MOTORS_NB) {
    chprintf (lchp, "Encf 1: %f\r\n", motors[motor_id].get_pos());
  }
}

static void cmd_odom(BaseSequentialStream *lchp, int argc,const char * const argv[]) {
  (void)argv;
  (void)argc;
  

    chprintf (lchp, "x : %f\r\n", odometry.get_x());
    chprintf (lchp, "y : %f\r\n", odometry.get_y());
    chprintf (lchp, "theta : %f\r\n", odometry.get_theta());
}


static void cmd_guidance(BaseSequentialStream *lchp, int argc,const char * const argv[]) {
  (void)argv;
  (void)argc;
  
  const Eigen::Vector3d pos {atof(argv[0]),atof(argv[1]),atof(argv[2])};  // 2pi = 1 tour
  const Eigen::Vector3d vitesse {atof(argv[3]),atof(argv[4]),atof(argv[5])};

  holocontrol.set_cons(pos,vitesse);
  chprintf (lchp, "Ok\r\n");
}


/*===========================================================================*/
/* START OF PRIVATE SECTION  : DO NOT CHANGE ANYTHING BELOW THIS LINE        */
/*===========================================================================*/

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/


#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(4096)

#if !defined CONSOLE_DEV_USB || CONSOLE_DEV_USB == false
static const SerialConfig ftdiConfig =  {
  115200,
  0,
  USART_CR2_STOP1_BITS | USART_CR2_LINEN,
  0
};
#endif


#define MAX_CPU_INFO_ENTRIES 20
typedef struct _ThreadCpuInfo {
  float    ticks[MAX_CPU_INFO_ENTRIES];
  float    cpu[MAX_CPU_INFO_ENTRIES];
  float    totalTicks;
  float    totalISRTicks;
} ThreadCpuInfo ;

#if CH_DBG_THREADS_PROFILING | CH_DBG_STATISTICS
static void stampThreadCpuInfo (ThreadCpuInfo *ti);
static float stampThreadGetCpuPercent (const ThreadCpuInfo *ti, const uint32_t idx) __attribute__((unused));
static float stampISRGetCpuPercent (const ThreadCpuInfo *ti);
#endif

static void cmd_uid(BaseSequentialStream *lchp, int argc,const char * const argv[]) {
  (void)argv;
  if (argc > 0) {
    chprintf (lchp, "Usage: uid\r\n");
    return;
  }

  /* for (uint32_t i=0; i<2000; i++) { */
  /*   bkpram[i] = (uint16_t) i; */
  /* } */
  
  /* for (uint32_t i=0; i<2000; i++) { */
  /*   if (bkpram[i] != (uint16_t) i) { */
  /*     DebugTrace ("bkpram error"); */
  /*   } */
  /* } */

  chprintf (lchp, "uniq id : ");
  for (uint32_t i=0; i< UniqProcessorIdLen; i++)
    chprintf (lchp, "[%x] ", UniqProcessorId[i]);
  chprintf (lchp, "\r\n");
}

// static void cmd_rtc(BaseSequentialStream *lchp, int argc,const char * const argv[])
// {
//   if ((argc != 0) && (argc != 2) && (argc != 6)) {
//      DebugTrace ("Usage: rtc [Hour Minute Second Year monTh Day day_of_Week Adjust] value or");
//      DebugTrace ("Usage: rtc  Hour Minute Second Year monTh Day");
//      return;
//   }
 
//   if (argc == 2) {
//     const char timeVar = (char) tolower ((int) *(argv[0]));
//     const int32_t varVal = strtol (argv[1], NULL, 10);
    
//     switch (timeVar) {
//     case 'h':
//       setHour ((uint32_t)(varVal));
//       break;
      
//     case 'm':
//        setMinute ((uint32_t)(varVal));
//       break;
      
//     case 's':
//       setSecond ((uint32_t)(varVal));
//       break;
      
//     case 'y':
//        setYear ((uint32_t)(varVal));
//       break;
      
//     case 't':
//        setMonth ((uint32_t)(varVal));
//       break;
      
//     case 'd':
//        setMonthDay ((uint32_t)(varVal));
//       break;

//     case 'w':
//        setWeekDay ((uint32_t)(varVal));
//       break;

//     case 'a':
//       {
// 	int32_t newSec =(int)(getSecond()) + varVal;
// 	if (newSec > 59) {
// 	  int32_t newMin =(int)(getMinute()) + (newSec/60);
// 	  if (newMin > 59) {
// 	    setHour ((getHour()+((uint32_t)(newMin/60))) % 24);
// 	    newMin %= 60;
// 	  }
// 	  setMinute ((uint32_t)newMin);
// 	}
// 	if (newSec < 0) {
// 	  int32_t newMin =(int)getMinute() + (newSec/60)-1;
// 	  if (newMin < 0) {
// 	    setHour ((getHour()-((uint32_t)newMin/60)-1) % 24);
// 	    newMin %= 60;
// 	  }
// 	  setMinute ((uint32_t)newMin);
// 	}
// 	setSecond ((uint32_t)newSec % 60);
//       }
//       break;
      
//     default:
//       DebugTrace ("Usage: rtc [Hour Minute Second Year monTh Day Weekday Adjust] value");
//     }
//   } else if (argc == 6) {
//     setYear ((uint32_t) atoi(argv[3]));
//     setMonth ((uint32_t) atoi(argv[4]));
//     setMonthDay ((uint32_t) atoi(argv[5]));
//     setHour ((uint32_t) atoi(argv[0]));
//     setMinute ((uint32_t) atoi(argv[1]));
//     setSecond ((uint32_t) atoi(argv[2]));
//   }

//   chprintf (lchp, "RTC : %s %.02lu/%.02lu/%.04lu  %.02lu:%.02lu:%.02lu\r\n",
// 	    getWeekDayAscii(), getMonthDay(), getMonth(), getYear(),
// 	    getHour(), getMinute(), getSecond());
// }


static void cmd_mem(BaseSequentialStream *lchp, int argc,const char * const argv[]) {
  (void)argv;
  if (argc > 0) {
    chprintf (lchp, "Usage: mem\r\n");
    return;
  }

  chprintf (lchp, "core free memory : %u bytes\r\n", chCoreGetStatusX());
  chprintf (lchp, "heap free memory : %u bytes\r\n", getHeapFree());

  void * ptr1 = malloc_m (100);
  void * ptr2 = malloc_m (100);

  chprintf (lchp, "(2x) malloc_m(1000) = %p ;; %p\r\n", ptr1, ptr2);
  chprintf (lchp, "heap free memory : %d bytes\r\n", getHeapFree());

  free_m (ptr1);
  free_m (ptr2);
}




#if CH_DBG_THREADS_PROFILING | CH_DBG_STATISTICS
static void cmd_threads(BaseSequentialStream *lchp, int argc,const char * const argv[]) {
  static const char *states[] = {CH_STATE_NAMES};
  thread_t *tp = chRegFirstThread();
  (void)argv;
  (void)argc;
  float totalTicks=0;
  float idleTicks=0;

  static ThreadCpuInfo threadCpuInfo = {
    .ticks = {0.f}, 
    .cpu =   {-1.f}, 
    .totalTicks = 0.f,
    .totalISRTicks = 0.f,
  };
  for(int i=0; i<MAX_CPU_INFO_ENTRIES; i++) {
    threadCpuInfo.cpu[i] = -1.f;
  }
  
  stampThreadCpuInfo (&threadCpuInfo);
  
  chprintf (lchp, "    addr    stack  frestk prio refs  state        time \t percent        name\r\n");
  uint32_t idx=0;
  do {
    chprintf (lchp, "%.8lx %.8lx %6lu %4lu %4lu %9s %9lu   %.2f%%    \t%s\r\n",
	      (uint32_t)tp, (uint32_t)tp->ctx.sp,
	      get_stack_free (tp),
	      (uint32_t)tp->hdr.pqueue.prio, (uint32_t)(tp->refs - 1),
	      states[tp->state],
#if CH_DBG_THREADS_PROFILING
	      (uint32_t)tp->time,
#elif CH_DBG_STATISTICS
	      (uint32_t)RTC2MS(STM32_SYSCLK, tp->stats.cumulative),
#endif
	      stampThreadGetCpuPercent (&threadCpuInfo, idx),
	      chRegGetThreadNameX(tp));
#if CH_DBG_THREADS_PROFILING
    totalTicks+= (float) tp->time;
#elif CH_DBG_STATISTICS
    totalTicks+= (float)tp->stats.cumulative;
#endif
    if (strcmp (chRegGetThreadNameX(tp), "idle") == 0)
#if CH_DBG_THREADS_PROFILING
      idleTicks =  (float) tp->time;
#elif CH_DBG_STATISTICS
    idleTicks = (float)tp->stats.cumulative;
#endif
    tp = chRegNextThread ((thread_t *)tp);
    idx++;
  } while (tp != NULL);

  const float idlePercent = (idleTicks*100.f)/totalTicks;
  const float cpuPercent = 100.f - idlePercent;
  chprintf (lchp, "Interrupt Service Routine \t\t     %9lu   %.2f%%    \tISR\r\n",
	    (uint32_t)RTC2MS(STM32_SYSCLK, threadCpuInfo.totalISRTicks),
	    stampISRGetCpuPercent(&threadCpuInfo));
  chprintf (lchp, "\r\ncpu load = %.2f%%\r\n", cpuPercent);
}
#else
static void cmd_threads(BaseSequentialStream *lchp, int argc,const char * const argv[]) {
  static const char *states[] = {THD_STATE_NAMES};
  thread_t *tp = chRegFirstThread();
  (void)argv;
  (void)argc;


  chprintf (lchp, "    addr    stack  frestk prio refs  state                name\r\n");
  do {
    chprintf (lchp, "%.8lx %.8lx %6lu %4lu %4lu %9s \t%s\r\n",
	      (uint32_t)tp, (uint32_t)tp->ctx.sp,
	      get_stack_free (tp),
	      (uint32_t)tp->hdr.pqueue.prio, (uint32_t)(tp->refs - 1),
	      states[tp->state], chRegGetThreadNameX(tp));
    tp = chRegNextThread ((thread_t *)tp);
  } while (tp != NULL);
}

#endif
static const ShellConfig shell_cfg1 = {
#if ! defined CONSOLE_DEV_USB || CONSOLE_DEV_USB == false
  (BaseSequentialStream *) &CONSOLE_DEV_SD,
#else
  (BaseSequentialStream *) &SDU1,
#endif
  commands
};



void consoleInit (void)
{
  /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * USBD1 : FS, USBD2 : HS
   */

#if defined CONSOLE_DEV_USB && CONSOLE_DEV_USB == true
  usbSerialInit(&SDU1, &USBD1); 
  chp = (BaseSequentialStream *) &SDU1;
#else
  sdStart(&CONSOLE_DEV_SD, &ftdiConfig);
  chp = (BaseSequentialStream *) &CONSOLE_DEV_SD;
#endif
  /*
   * Shell manager initialization.
   */
  shellInit();
}


void consoleLaunch (void)
{
  thread_t *shelltp = NULL;

#if CONSOLE_DEV_USB != 0
   while (TRUE) {
    if (!shelltp) {
      systime_t time=90;


      while (usbGetDriver()->state != USB_ACTIVE) {
	if (time != 100) {
	  time++;
	  chThdSleepMilliseconds(100);
	} else {
	  time=90;
	  //usbSerialReset(&SDU1);
	}
      }
      
      // activate driver, giovani workaround
      chnGetTimeout(&SDU1, TIME_IMMEDIATE);
      while (!isUsbConnected()) {
	chThdSleepMilliseconds(100);
      }
      
      shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
    } else if (shelltp && (chThdTerminatedX(shelltp))) {
      chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
      shelltp = NULL;           /* Triggers spawning of a new shell.        */
    }
    chThdSleepMilliseconds(100);
  }

#else
  if (!shelltp) {
    //       palSetPad (BOARD_LED3_P, BOARD_LED3);
    shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
  } else if (chThdTerminatedX(shelltp)) {
    chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
    shelltp = NULL;           /* Triggers spawning of a new shell.        */
    //       palClearPad (BOARD_LED3_P, BOARD_LED3);
  }
#endif //CONSOLE_DEV_USB
}

#if CH_DBG_THREADS_PROFILING
static void stampThreadCpuInfo (ThreadCpuInfo *ti)
{
  const thread_t *tp =  chRegFirstThread();
  uint32_t idx=0;
  
  float totalTicks =0;
  do {
    totalTicks+= (float) tp->time;
    ti->cpu[idx] = (float) tp->time - ti->ticks[idx];;
    ti->ticks[idx] = (float) tp->time;
    tp = chRegNextThread ((thread_t *)tp);
    idx++;
  } while ((tp != NULL) && (idx < MAX_CPU_INFO_ENTRIES));
  
  const float diffTotal = totalTicks- ti->totalTicks;
  ti->totalTicks = totalTicks;
  
  tp =  chRegFirstThread();
  idx=0;
  do {
    ti->cpu[idx] =  (ti->cpu[idx]*100.f)/diffTotal;
    tp = chRegNextThread ((thread_t *)tp);
    idx++;
  } while ((tp != NULL) && (idx < MAX_CPU_INFO_ENTRIES));
}

static float stampISRGetCpuPercent (const ThreadCpuInfo *)
{
  return -1.0f;
}

#elif CH_DBG_STATISTICS
static void stampThreadCpuInfo (ThreadCpuInfo *ti)
{
  const thread_t *tp =  chRegFirstThread();
  uint32_t idx=0;
  
  ti->totalTicks =0;
  do {
    ti->ticks[idx] = (float) tp->stats.cumulative;
    ti->totalTicks += ti->ticks[idx];
    tp = chRegNextThread ((thread_t *)tp);
    idx++;
  } while ((tp != NULL) && (idx < MAX_CPU_INFO_ENTRIES));
  ti->totalISRTicks = currcore->kernel_stats.m_crit_isr.cumulative;
  ti->totalTicks += ti->totalISRTicks;
  tp =  chRegFirstThread();
  idx=0;
  do {
    ti->cpu[idx] =  (ti->ticks[idx]*100.f) / ti->totalTicks;
    tp = chRegNextThread ((thread_t *)tp);
    idx++;
  } while ((tp != NULL) && (idx < MAX_CPU_INFO_ENTRIES));
}

static float stampThreadGetCpuPercent (const ThreadCpuInfo *ti, const uint32_t idx)
{
  if (idx >= MAX_CPU_INFO_ENTRIES) 
    return -1.f;

  return ti->cpu[idx];
}
static float stampISRGetCpuPercent (const ThreadCpuInfo *ti)
{
  return ti->totalISRTicks * 100.0f / ti->totalTicks;
}
#endif
