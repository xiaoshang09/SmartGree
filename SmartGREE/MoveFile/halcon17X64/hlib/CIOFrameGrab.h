/*****************************************************************************
 * CIOFrameGrab.h
 *****************************************************************************
 *
 * Project:     HALCON/libhalcon
 * Description: Image acquisition integration interface
 *
 * (c) 1996-2017 by MVTec Software GmbH
 *                  www.mvtec.com
 *
 *****************************************************************************
 *
 *
 *****************************************************************************
 *
 * See also:  "Image Acquisition Integration Programmer's Manual"
 *
 *****************************************************************************
 */


#ifndef CIOFRAMEGRAB_H
#define CIOFRAMEGRAB_H

#ifndef HC_NO_FRAMEGRABBER

#include "CIOFrameGrabDefines.h"


/* This Macro checks the syntax of Generic.                                  *
 * Call it within the loop over the tuple of Generic parameters.             *
 * i          index of parameter                                             *
 * pos        char *, that points to the "="                                 *
 * ret_act_t  return action in case of wrong type (H_ERR_FGPART)             *
 * ret_act_v  return action in case of wrong value (H_ERR_FGPARV)            */
#  define CHECK_GENERIC(i,pos,ret_act_t,ret_act_v) { \
  if (fginst->generic[(i)].type == STRING_PAR) \
  { \
    if (!strlen(fginst->generic[i].par.s)) \
      continue; \
    if ((pos) = strstr(fginst->generic[(i)].par.s,"=")) \
    { \
      if (strstr((pos) + 1,"=")) \
      { \
        sprintf(errMsg,"Error: More than one '=' found in '%s'. " \
                       "Please make sure to use a syntax like this " \
                       "for multiple parameters: " \
                       "['par1=arg1', 'par2=arg2', ...]", \
                fginst->generic[i].par.s); \
        MY_PRINT_ERROR_MESSAGE(errMsg); \
        ret_act_v; \
      } \
    } \
    else \
    { \
      sprintf(errMsg,"Error: No '=' found in '%s'. " \
                     "Please make sure to use a syntax like this: " \
                     "'parameter=argument'", \
              fginst->generic[i].par.s); \
      MY_PRINT_ERROR_MESSAGE(errMsg); \
      ret_act_v; \
    } \
  } \
  else \
  { \
    if (fginst->num_generic == 1 && \
         ((fginst->generic[(i)].type == LONG_PAR && \
           fginst->generic[(i)].par.l == -1) || \
          (fginst->generic[(i)].type == FLOAT_PAR && \
           fginst->generic[(i)].par.f == -1.0))) \
      break; \
    else \
    { \
      MY_PRINT_ERROR_MESSAGE("Generic parameter accepts " \
                             "only string values!"); \
      ret_act_t; \
    } \
  } \
}


#  if defined(__cplusplus)
extern "C" {
#  endif

/* function for integrating image acquisition devices into the system using
   a linked interface (instead of a dynamic object accessed online) */
extern HLibExport Herror HAddFgClass(Hproc_handle proc_id, char *name,
                                     FG_INIT_FUNC init_func);

/* function for writing the interface specific default values into an
   Hcpar array */
extern HLibExport Herror HFgGetDefaults(Hproc_handle proc_id, FGClass *fgClass,
                                        Hcpar **values, HINT *numValues);

/* helper function in HALCON library for displaying low-level error messages */
extern HLibExport Herror IOPrintErrorMessage(char *err);

/* prototypes to be included in global startup and shutdown */
Herror HStartupFramegrabberInterface(void);
Herror HShutdownFramegrabberInterface(void);

#  if defined(__cplusplus)
}
#  endif

#endif /* ifndef HC_NO_FRAMEGRABBER */

#endif /* ifndef CIOFRAMEGRAB_H */
