
#ifdef WIN32

#include <windows.h>
typedef char bool;
#define false 0
#define true 1 
#define OSDECL __declspec(dllexport)

#else

#define OSDECL

#endif

#ifndef HIBYTE
#define HIBYTE(x) ((((uint32_t)(x)) & 0xff00) >> 8)
#endif

#include <stdint.h>
#include "npapi.h"
#include "npfunctions.h"
#include "npruntime.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static NPNetscapeFuncs *npnfuncs;

//lazy
void _npapi_set_string(NPVariant *v, char *txt, int length)
{ 
	char *t = (char *)npnfuncs->memalloc(length);
	strcpy(t, txt);
	STRINGN_TO_NPVARIANT(t, length, *v);
}

char *_npapi_get_string(NPVariant *v)
{ 
	return (char*)NPVARIANT_TO_STRING(*v).UTF8Characters;
}


typedef struct BMX
{
  NPObject np;
  void *ptr;
} BMX;

NPObject *BMX_Allocate(NPP npp, NPClass *aClass)
{
  BMX*object = (BMX*)malloc(sizeof(BMX));
  object->ptr = NULL;
  return &object->np;
}

static bool BMX_HasMethod(NPObject* obj, NPIdentifier methodName) {
	BMX *bmx = (BMX*)obj;
	char *name = npnfuncs->utf8fromidentifier(methodName);
	int result = _prime_npapi_TNPAPIObject_OnHasMethod(bmx->ptr, name);
	npnfuncs->memfree(name);
	return result;
}

static bool BMX_InvokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	BMX *bmx = (BMX*)obj;
	return _prime_npapi_TNPAPIObject_OnInvoke(bmx->ptr, NULL, args, argCount, result);;
}

static bool BMX_Invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	BMX *bmx = (BMX*)obj;
	char *name = npnfuncs->utf8fromidentifier(methodName);
	int res = _prime_npapi_TNPAPIObject_OnInvoke(bmx->ptr, name, args, argCount, result);;
	npnfuncs->memfree(name);
	return res;
}

static bool BMX_HasProperty(NPObject *obj, NPIdentifier propertyName) {
	return false;
}

static bool BMX_GetProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result) {
	return false;
}

NPClass bmxObject = {
	NP_CLASS_STRUCT_VERSION,
	BMX_Allocate,
	NULL,
	NULL,
	BMX_HasMethod,
	BMX_Invoke,
	BMX_InvokeDefault,
	BMX_HasProperty,
	BMX_GetProperty,
	NULL,
	NULL,
};

static NPError BMX_New(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char *argn[], char *argv[], NPSavedData *saved) {
	instance->pdata = (void*)_prime_npapi_TNPAPIPlugin_OnNew(pluginType, argc, argn, argv);
	if (instance->pdata == NULL)
		return NPERR_GENERIC_ERROR;
	
	//npnfuncs->setvalue(instance, NPPVpluginWindowBool, (void*)false);

	return NPERR_NO_ERROR;
}

static NPError BMX_Destroy(NPP instance, NPSavedData **save) {
	if (instance->pdata != NULL)
		_prime_npapi_TNPAPIPlugin_OnDestroy(instance->pdata);
	return NPERR_NO_ERROR;
}

static NPError BMX_GetValue(NPP instance, NPPVariable variable, void *value) {
	BMX *bmx = NULL;
	int so = 0;

	switch(variable) {
	default:
		return NPERR_GENERIC_ERROR;
	case NPPVpluginNameString:
		*((char **)value) = (char*)_prime_npapi_TNPAPIPlugin_GetName();
		break;
	case NPPVpluginDescriptionString:
		*((char **)value) = (char*)_prime_npapi_TNPAPIPlugin_GetDescription();
		break;
	case NPPVpluginScriptableNPObject:		
		so = _prime_npapi_TNPAPIObject_GetData(instance->pdata, &bmx);  	
		if (bmx == NULL && so)
		{
			bmx = (BMX*)npnfuncs->createobject(instance, &bmxObject);
			npnfuncs->retainobject((NPObject*)bmx);
			bmx->ptr = instance->pdata;
			_prime_npapi_TNPAPIObject_SetData(instance->pdata, bmx);
		}
		*(BMX **)value = bmx;
		break;
	case NPPVpluginNeedsXEmbed:
		*((int *)value) = 1;
		break;
	}
	return NPERR_NO_ERROR;
}

static NPError OSCALL BMX_HandleEvent(NPP instance, void *ev) {
	NPEvent *event = (NPEvent*)ev;
#ifdef WIN32
	_prime_npapi_TNPAPIObject_OnHandleEvent(instance->pdata, event->event, event->wParam, event->lParam);
#endif
	return NPERR_NO_ERROR;
}

static NPError OSCALL BMX_SetWindow(NPP instance, NPWindow* window) {
#ifdef WIN32
	HWND hwnd;
	npnfuncs->getvalue(instance, NPNVnetscapeWindow, (void*) &hwnd);
	_prime_npapi_TNPAPIObject_OnSetWindow(instance->pdata, window->window);
	_prime_npapi_TNPAPIObject_OnSetWindow(instance->pdata, hwnd);
#endif
	return NPERR_NO_ERROR;
}

NPError OSCALL NP_GetEntryPoints(NPPluginFuncs *nppfuncs) {
	if(nppfuncs == NULL)
    return NPERR_INVALID_FUNCTABLE_ERROR;

  if(nppfuncs->size < sizeof(NPPluginFuncs))
    return NPERR_INVALID_FUNCTABLE_ERROR;

	nppfuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
	nppfuncs->newp          = BMX_New;
	nppfuncs->destroy       = BMX_Destroy;
	nppfuncs->getvalue      = BMX_GetValue;
	nppfuncs->event         = BMX_HandleEvent;
	nppfuncs->setwindow     = BMX_SetWindow;
	return NPERR_NO_ERROR;
}

OSDECL NPError OSCALL NP_Initialize(NPNetscapeFuncs *npnf 
#ifdef WIN32
                             )
#else
                             ,NPPluginFuncs *nppfuncs)
#endif
{
	if(npnf == NULL)
		return NPERR_INVALID_FUNCTABLE_ERROR;

	if(HIBYTE(npnf->version) > NP_VERSION_MAJOR)
		return NPERR_INCOMPATIBLE_VERSION_ERROR;
    	

	npnfuncs = npnf;
#ifndef WIN32
	NP_GetEntryPoints(nppfuncs);
#endif
	return NPERR_NO_ERROR;
}

NPError OSCALL NP_Shutdown() {
	_prime_npapi_TNPAPIPlugin_OnShutdown();
	return NPERR_NO_ERROR;
}

#if NP_VERSION_MINOR >= 27
const 
#endif
char *NP_GetMIMEDescription(void)
{
	return (char*)_prime_npapi_TNPAPIPlugin_GetMIMEDescription();
}

NPError OSCALL NP_GetValue(void *npp, NPPVariable variable, void *value)
{
	return BMX_GetValue((NPP)npp, variable, value);
}
