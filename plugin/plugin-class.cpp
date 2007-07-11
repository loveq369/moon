/*
 * plugin-class.cpp: MoonLight browser plugin.
 *
 * Author:
 *   Everaldo Canuto (everaldo@novell.com)
 *
 * Copyright 2007 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 * 
 */

#include <ctype.h>
#include "plugin-class.h"
#include "plugin.h"

/*** Static wrapper functions *************************************************/

NPObject*
MoonlightClass::moonlightAllocate (NPP instance, NPClass *aClass)
{
	return (NPObject*)((MoonlightClass *) aClass)->AllocateObject (instance);
}

void
MoonlightClass::moonlightDeallocate (NPObject *npobj)
{
	((MoonlightClass*)npobj->_class)->DeallocateObject ((MoonlightObject*)npobj);
}

void
MoonlightClass::moonlightInvalidate (NPObject *npobj)
{
	((MoonlightClass *) npobj->_class)->InvalidateObject ((MoonlightObject *) npobj);
}

bool
MoonlightClass::moonlightHasProperty (NPObject *npobj, NPIdentifier name)
{
	return ((MoonlightClass *) npobj->_class)->HasProperty ((MoonlightObject *) npobj, name);
}

bool
MoonlightClass::moonlightGetProperty (NPObject *npobj, NPIdentifier name, NPVariant *result)
{
	return ((MoonlightClass *) npobj->_class)->GetProperty ((MoonlightObject *) npobj, name, result);
}

bool
MoonlightClass::moonlightSetProperty (NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
	return ((MoonlightClass *) npobj->_class)->SetProperty ((MoonlightObject *) npobj, name, value);
}

bool
MoonlightClass::moonlightRemoveProperty (NPObject *npobj, NPIdentifier name)
{
	return ((MoonlightClass *) npobj->_class)->RemoveProperty ((MoonlightObject *) npobj, name);
}

bool
MoonlightClass::moonlightHasMethod (NPObject *npobj, NPIdentifier name)
{
	return ((MoonlightClass *) npobj->_class)->HasMethod ((MoonlightObject *) npobj, name);
}

bool
MoonlightClass::moonlightInvoke (NPObject *npobj, NPIdentifier name, const NPVariant *args, 
				 uint32_t argCount, NPVariant *result)
{
	return ((MoonlightClass *) npobj->_class)->Invoke ((MoonlightObject *) npobj, name, args, argCount, result);
}

bool
MoonlightClass::moonlightInvokeDefault (NPObject *npobj, const NPVariant *args,
					uint32_t argCount, NPVariant *result)
{
	return ((MoonlightClass *) npobj->_class)->InvokeDefault ((MoonlightObject *) npobj, args, argCount, result);
}

/*** MoonlightClass **************************************************************/

MoonlightClass::MoonlightClass ()
{
	this->allocate       = moonlightAllocate;
	this->deallocate     = moonlightDeallocate;
	this->invalidate     = moonlightInvalidate;
	this->hasProperty    = moonlightHasProperty;
	this->getProperty    = moonlightGetProperty;
	this->setProperty    = moonlightSetProperty;
	this->removeProperty = moonlightRemoveProperty;
	this->hasMethod      = moonlightHasMethod;
	this->invoke         = moonlightInvoke;
	this->invokeDefault  = moonlightInvokeDefault;
}

MoonlightClass::~MoonlightClass ()
{
	// nothing to do.
}

MoonlightObject*
MoonlightClass::AllocateObject (NPP instance)
{
	return new MoonlightObject (instance);
}

void
MoonlightClass::DeallocateObject (MoonlightObject *npobj)
{
  // XXX this is crashing?  bad....
  //	delete npobj;
}

void
MoonlightClass::InvalidateObject (MoonlightObject *npobj)
{
	// nothing to do.
}

bool
MoonlightClass::HasProperty (MoonlightObject *npobj, NPIdentifier name)
{
	NPUTF8 * strname = NPN_UTF8FromIdentifier (name);
	DEBUGMSG ("*** MoonlightClass::HasProperty %s", strname);
	NPN_MemFree(strname);

	return false;
}

bool
MoonlightClass::GetProperty (MoonlightObject *npobj, NPIdentifier name, NPVariant *result)
{
	DEBUGMSG ("*** MoonlightClass::GetProperty");
	return false;
}

bool
MoonlightClass::SetProperty (MoonlightObject *npobj, NPIdentifier name, const NPVariant *value)
{
	DEBUGMSG ("*** MoonlightClass::SetProperty");
	return false;
}

bool
MoonlightClass::RemoveProperty (MoonlightObject *npobj, NPIdentifier name)
{
	return false;
}

bool
MoonlightClass::HasMethod (MoonlightObject *npobj, NPIdentifier name)
{
	NPUTF8 * strname = NPN_UTF8FromIdentifier (name);
	DEBUGMSG ("*** MoonlightClass::HasMethod %s", strname);
	NPN_MemFree(strname);

	return false;
}

bool
MoonlightClass::Invoke (MoonlightObject *npobj, NPIdentifier name, const NPVariant *args, 
			uint32_t argCount, NPVariant *result)
{
	DEBUGMSG ("*** MoonlightClass::Invoke");
	return false;
}

bool
MoonlightClass::InvokeDefault (MoonlightObject *npobj, const NPVariant *args,
			       uint32_t argCount, NPVariant *result)
{
	DEBUGMSG ("*** MoonlightClass::InvokeDefault");
	return false;
}

int
MoonlightClass::IndexOf (NPIdentifier name, const char *const names[], int count)
{
	for (int i = 0; i < count; i++) {
		if (name == NPN_GetStringIdentifier (names [i]))
			return i;
	}

	return -1;
}

void
MoonlightClass::StringToNPVariant (char *value, NPVariant *result)
{
	int len;
	char * retval;

	if (value) {
		len = strlen (value);
		retval = (char *) NPN_MemAlloc (len + 1);
		strcpy (retval, value);
	} else {
		len = 0;
		retval = (char *) NPN_MemAlloc (1);
		retval[0] = 0;
	}

	STRINGN_TO_NPVARIANT (retval, len, *result);
}

/*** MoonlightControlClass **********************************************************/

MoonlightControlClass* MoonlightControlClass::_class = NULL;

MoonlightControlClass::MoonlightControlClass ()
{
}

const char *const MoonlightControlClass::properties[] = {
	"settings",   // read only
	"content",    // read only
	"initParams", // read only
	"isLoaded",   // read only
	"source"      // read write (cant be set after initialization)
};
const char *const MoonlightControlClass::methods[] = { };

MoonlightObject*
MoonlightControlClass::AllocateObject (NPP instance)
{
	return new MoonlightControlObject (instance);
}

void
MoonlightControlClass::InvalidateObject (MoonlightObject *npobj)
{
	MoonlightControlObject *control = (MoonlightControlObject*)npobj;

	if (control->settings)
		NPN_ReleaseObject (control->settings);
	control->settings = NULL;

	if (control->content)
		NPN_ReleaseObject (control->content);
	control->content = NULL;
}

bool
MoonlightControlClass::HasProperty (MoonlightObject *npobj, NPIdentifier name)
{
  DEBUGMSG ("MoonlightControlClass::HasProperty");
	return HAS_PROPERTY (properties, name);
}

bool
MoonlightControlClass::HasMethod (MoonlightObject *npobj, NPIdentifier name)
{
  DEBUGMSG ("MoonlightControlClass::HasMethod");
	return HAS_METHOD (methods, name);
}

bool
MoonlightControlClass::GetProperty (MoonlightObject *npobj, NPIdentifier name, NPVariant *result)
{
	PluginInstance *plugin = (PluginInstance*) npobj->instance->pdata;
	MoonlightControlObject *rootobj = (MoonlightControlObject*)npobj;

	if (name == NPID ("settings")) {
		OBJECT_TO_NPVARIANT (rootobj->settings, *result);
		return true;
	} 

	if (name == NPID ("content")) {
		OBJECT_TO_NPVARIANT (rootobj->content, *result);
		return true;
	} 

	if (name == NPID ("initParams")) {
		StringToNPVariant (plugin->getInitParams (), result);
		return true;
	} 

	if (name == NPID ("isLoaded")) {
		BOOLEAN_TO_NPVARIANT (plugin->getIsLoaded (), *result);
		return true;
	} 

	if (name == NPID ("source")) {
		StringToNPVariant (plugin->getSource (), result);
		return true;
	} 

	return false;
}

bool 
MoonlightControlClass::SetProperty (MoonlightObject *npobj, NPIdentifier name, const NPVariant *value)
{
	PluginInstance *plugin = (PluginInstance*) npobj->instance->pdata;

	if (name == NPID ("source")) {
		plugin->setSource (NPVARIANT_TO_STRING (*value).utf8characters);
		return true;
	}

	return false;
}

bool
MoonlightControlClass::Invoke (MoonlightObject *npobj, NPIdentifier name, 
			       const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return false;
}

/*** MoonlightSettingsClass ***********************************************************/

MoonlightSettingsClass* MoonlightSettingsClass::_class = NULL;

MoonlightSettingsClass::MoonlightSettingsClass ()
{
}

const char *const MoonlightSettingsClass::properties [] = {
	"background",             // read write
	"enableFramerateCounter", // read write (cant be set after initialization)
	"enableRedrawRegions",    // read write
	"enableHtmlAccess",       // read write (cant be set after initialization)
	"maxFrameRate",           // read write
	"version",                // read only
	"windowless"              // read write (cant be set after initialization)
};

bool
MoonlightSettingsClass::HasProperty (MoonlightObject *npobj, NPIdentifier name)
{
	return HAS_PROPERTY (properties, name);
}

bool
MoonlightSettingsClass::GetProperty (MoonlightObject *npobj, NPIdentifier name, NPVariant *result)
{
	PluginInstance *plugin = (PluginInstance*) npobj->instance->pdata;

	if (name == NPID ("background")) {
		StringToNPVariant (plugin->getBackground (), result);
		return true;
	}

	if (name == NPID ("enableFramerateCounter")) {
		BOOLEAN_TO_NPVARIANT (plugin->getEnableFramerateCounter (), *result);
		return true;
	} 

	if (name == NPID ("enableRedrawRegions")) {
		BOOLEAN_TO_NPVARIANT (plugin->getEnableRedrawRegions (), *result);
		return true;
	} 

	if (name == NPID ("enableHtmlAccess")) {
		BOOLEAN_TO_NPVARIANT (plugin->getEnableHtmlAccess (), *result);
		return true;
	} 

	// not implemented yet, just return 0.
	if (name == NPID ("maxFrameRate")) {
		INT32_TO_NPVARIANT (0, *result);
		return true;
	}

	if (name == NPID ("version")) {
		StringToNPVariant (PLUGIN_VERSION, result);
		return true;
	}

	if (name == NPID ("windowless")) {
		BOOLEAN_TO_NPVARIANT (plugin->getWindowless (), *result);
		return true;
	} 

	return false;
}

bool 
MoonlightSettingsClass::SetProperty (MoonlightObject *npobj, NPIdentifier name, const NPVariant *value)
{
	PluginInstance *plugin = (PluginInstance*) npobj->instance->pdata;
	if (name == NPID ("background")) {
		plugin->setBackground (NPVARIANT_TO_STRING (*value).utf8characters);
		return true;
	}

	// Cant be set after initialization so return true
	if (name == NPID ("enableFramerateCounter")) {
		return true;
	} 

	if (name == NPID ("enableRedrawRegions")) {
		plugin->setEnableRedrawRegions (NPVARIANT_TO_BOOLEAN (*value));
		return true;
	} 

	// Cant be set after initialization so return true
	if (name == NPID ("enableHtmlAccess")) {
		return true;
	} 

	// not implemented yet.
	if (name == NPID ("maxFrameRate")) {
		return true;
	}

	// Cant be set after initialization so return true
	if (name == NPID ("windowless")) {
		return true;
	} 

	return false;
}

/*** MoonlightContentClass ************************************************************/

MoonlightContentClass* MoonlightContentClass::_class = NULL;

MoonlightContentClass::MoonlightContentClass ()
{
}


const char *const MoonlightContentClass::properties[] = {
	"actualHeight", // read only
	"actualWidth",  // read only
	"fullScreen"    // read write
};

const char *const MoonlightContentClass::methods[] = {
	"createObject",
	"createFromXaml",
	"createFromXamlDownloader"
};

bool
MoonlightContentClass::HasProperty (MoonlightObject *npobj, NPIdentifier name)
{
	return HAS_PROPERTY (properties, name);
}

bool
MoonlightContentClass::HasMethod (MoonlightObject *npobj, NPIdentifier name)
{
	return HAS_METHOD (methods, name);
}

bool
MoonlightContentClass::GetProperty (MoonlightObject *npobj, NPIdentifier name, NPVariant *result)
{
	PluginInstance *plugin = (PluginInstance*) npobj->instance->pdata;

	// Silverlight always return 0.
	if (name == NPID ("actualHeight")) {
		INT32_TO_NPVARIANT (plugin->getActualHeight (), *result);
		return true;
	}

	// Silverlight always return 0.
	if (name == NPID ("actualWidth")) {
		INT32_TO_NPVARIANT (plugin->getActualWidth (), *result);
		return true;
	}

	// not implemented yet.
	if (name == NPID ("fullScreen")) {
		BOOLEAN_TO_NPVARIANT (false, *result);
		return true;
	}

	return false;
}

bool 
MoonlightContentClass::SetProperty (MoonlightObject *npobj, NPIdentifier name, const NPVariant *value)
{
	// not implemented yet.
	if (name == NPID ("fullScreen")) {
		return true;
	}

	return false;
}

bool
MoonlightContentClass::Invoke (MoonlightObject *npobj, NPIdentifier name, 
			       const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	if (name == NPID ("createObject")) {
		// not implemented yet
		return true;
	}
	else if (name == NPID ("createFromXaml")) {
		// create a Control object

		if (argCount < 1)
			return true;

		char *xaml = (char *) NPVARIANT_TO_STRING (args[0]).utf8characters;

		Control *control = new Control ();
		Type::Kind element_type;
		control->InitializeFromXaml (xaml, &element_type);

		MoonlightDependencyObjectObject *depobj =
			MoonlightDependencyObjectClass::CreateWrapper (npobj->instance, control);

		OBJECT_TO_NPVARIANT (depobj, *result);

		return true;
	}
	else if (name == NPID ("createFromXamlDownloader")) {
		// not implemented yet
		return true;
	}

	return false;
}

/*** MoonlightDependencyObjectClass ***************************************************/

MoonlightDependencyObjectClass* MoonlightDependencyObjectClass::_class = NULL;

const char *const MoonlightDependencyObjectClass::properties [] = {
};

const char *const MoonlightDependencyObjectClass::methods [] = {
	"getHost",
	"addEventListener",
	"removeEventListener",
	"findName"
};

void
value_to_variant (MoonlightObject *npobj, Value *v, NPVariant *result)
{
	switch (v->GetKind ()) {
	case Type::BOOL:
		BOOLEAN_TO_NPVARIANT (v->AsBool(), *result);
		break;

	case Type::INT32:
		INT32_TO_NPVARIANT (v->AsInt32(), *result);
		break;

	case Type::DOUBLE:
		DOUBLE_TO_NPVARIANT (v->AsDouble(), *result);
		break;
	case Type::STRING:
		STRINGZ_TO_NPVARIANT (v->AsString(), *result);
		break;

	/* more builtins.. */
	default:
		if (v->GetKind () >= Type::DEPENDENCY_OBJECT) {
			MoonlightDependencyObjectObject *depobj =
				MoonlightDependencyObjectClass::CreateWrapper (npobj->instance, v->AsDependencyObject ());
				OBJECT_TO_NPVARIANT (depobj, *result);
		}
		break;
	}
}

MoonlightDependencyObjectClass::MoonlightDependencyObjectClass ()
	: MoonlightClass ()
{
}

MoonlightObject *
MoonlightDependencyObjectClass::AllocateObject (NPP instance)
{
	return new MoonlightDependencyObjectObject (instance);
}

void
MoonlightDependencyObjectClass::InvalidateObject (MoonlightObject *npobj)
{
	MoonlightDependencyObjectObject *depobj = (MoonlightDependencyObjectObject*)npobj;

	if (depobj->dob)
		depobj->dob->unref ();
	depobj->dob = NULL;
}

DependencyProperty*
MoonlightDependencyObjectClass::GetDependencyProperty (DependencyObject *obj, char *attrname)
{
  DEBUGMSG ("GetDependencyProperty (%s)", attrname);

	attrname[0] = toupper(attrname[0]);
	DependencyProperty *p = obj->GetDependencyProperty (attrname);

	if (p)
		return p;

	char *period = strchr (attrname, '.');
	if (period) {
		char *type_name = g_strndup (attrname, period-attrname);
		attrname = period + 1;

		Type* type = Type::Find (type_name);
		if (type != NULL) {
			p = dependency_property_lookup (type->type, attrname);
		}
		g_free (type_name);
	}

	return p;
}

bool
MoonlightDependencyObjectClass::HasProperty (MoonlightObject *npobj, NPIdentifier name)
{
	if (HAS_PROPERTY (properties, name))
		return true;

	DependencyObject *dob = ((MoonlightDependencyObjectObject*)npobj)->dob;

	NPUTF8 *strname = NPN_UTF8FromIdentifier (name);
	DEBUGMSG ("MoonlightDependencyObjectClass::HasProperty (%s)", strname);
	DependencyProperty *p = GetDependencyProperty (dob, strname);
	NPN_MemFree (strname);

	return (p != NULL);
}

bool
MoonlightDependencyObjectClass::GetProperty (MoonlightObject *npobj, NPIdentifier name, NPVariant *result)
{
	DependencyObject *dob = ((MoonlightDependencyObjectObject*)npobj)->dob;

	NPUTF8 *strname = NPN_UTF8FromIdentifier (name);
	strname[0] = toupper(strname[0]);
	DependencyProperty *p = GetDependencyProperty (dob, strname);
	NPN_MemFree (strname);

	if (!p)
		return false;

	Value *value = dob->GetValue (p);
	if (!value)
		return false;

	value_to_variant (npobj, value, result);

	return true;
}

bool 
MoonlightDependencyObjectClass::SetProperty (MoonlightObject *npobj, NPIdentifier name, const NPVariant *value)
{
	DependencyObject *dob = ((MoonlightDependencyObjectObject*)npobj)->dob;

	NPUTF8 *strname = NPN_UTF8FromIdentifier (name);
	strname[0] = toupper(strname[0]);
	DependencyProperty *p = GetDependencyProperty (dob, strname);
	NPN_MemFree (strname);

	if (!p)
		return false;

	char *strvalue = (char *) NPN_MemAlloc (20);

	if (NPVARIANT_IS_BOOLEAN (*value)) {
		strcpy (strvalue, (NPVARIANT_TO_BOOLEAN (*value) ? "true" : "false"));
	} else if (NPVARIANT_IS_INT32 (*value)) {
		sprintf (strvalue, "%d", NPVARIANT_TO_INT32 (*value));
	} else if (NPVARIANT_IS_DOUBLE (*value)) {
		sprintf (strvalue, "%d", NPVARIANT_TO_DOUBLE (*value));
	} else if (NPVARIANT_IS_OBJECT(*value)) {
		// not implemented yet.
	}

	if (NPVARIANT_IS_STRING (*value))
		xaml_set_property_from_str (dob, p, (char *) NPVARIANT_TO_STRING (*value).utf8characters);
	else
		xaml_set_property_from_str (dob, p, strvalue);

	NPN_MemFree (strvalue);

	return true;
}

bool
MoonlightDependencyObjectClass::HasMethod (MoonlightObject *npobj, NPIdentifier name)
{
	NPUTF8 *strname = NPN_UTF8FromIdentifier (name);
	DEBUGMSG ("MoonlightDependencyObjectClass::HasMethod (%s)", strname);
	NPN_MemFree (strname);

	if (HAS_METHOD (methods, name))
		return true;

	return false;
}

bool
MoonlightDependencyObjectClass::Invoke (MoonlightObject *npobj, NPIdentifier name,
					const NPVariant *args, uint32_t argCount,
					NPVariant *result)
{
	DependencyObject *dob = ((MoonlightDependencyObjectObject*)npobj)->dob;

	if (name == NPID ("findName")) {
		if (!argCount)
			return true;

		char *name = (char *) NPVARIANT_TO_STRING (args[0]).utf8characters;
		
		DependencyObject *element = dob->FindName (name);
		if (!element)
			return true;

		MoonlightDependencyObjectObject *depobj = MoonlightDependencyObjectClass::CreateWrapper (npobj->instance, element);

		OBJECT_TO_NPVARIANT (depobj, *result);
		return true;
	}
	else if (name == NPID ("getHost")) {
		PluginInstance *plugin = (PluginInstance*) npobj->instance->pdata;

		OBJECT_TO_NPVARIANT ((NPObject*)plugin->getRootObject(), *result);
	}
	else if (name == NPID ("addEventListener")) {
		// not yet implemented
		return true;
	}
	else if (name == NPID ("removeEventlistener")) {
		// not yet implemented
		return true;
	}
	else
		return MoonlightClass::Invoke (npobj, name,
					       args, argCount,
					       result);
}

MoonlightDependencyObjectObject*
MoonlightDependencyObjectClass::CreateWrapper (NPP instance, DependencyObject *obj)
{
	NPClass *np_class = MoonlightDependencyObjectClass::Class();

	/* for DependencyObject subclasses which have special plugin classes, check here */
	if (Type::Find (obj->GetObjectType ())->IsSubclassOf (Type::COLLECTION))
		np_class = MoonlightCollectionClass::Class();
	else if (obj->GetObjectType() == Type::STORYBOARD)
		np_class = MoonlightStoryboardClass::Class();

	MoonlightDependencyObjectObject *depobj
		= (MoonlightDependencyObjectObject*)NPN_CreateObject (instance,
								      np_class);

	depobj->SetDependencyObject (obj);

	return depobj;
}


/*** MoonlightCollectionClass ***************************************************/

MoonlightCollectionClass* MoonlightCollectionClass::_class = NULL;

const char *const MoonlightCollectionClass::properties [] = {
	"count"
};

const char *const MoonlightCollectionClass::methods [] = {
	"add",
	"remove",
	"insert",
	"clear",
	"getItem"
};

MoonlightCollectionClass::MoonlightCollectionClass ()
	: MoonlightDependencyObjectClass ()
{
}

bool
MoonlightCollectionClass::HasProperty (MoonlightObject *npobj, NPIdentifier name)
{
  DEBUGMSG ("MoonlightCollectionClass::HasProperty");

	if (HAS_PROPERTY (properties, name))
		return true;

	return MoonlightDependencyObjectClass::HasProperty (npobj, name);
}

bool
MoonlightCollectionClass::GetProperty (MoonlightObject *npobj, NPIdentifier name, NPVariant *result)
{
	Collection *col = (Collection*)((MoonlightDependencyObjectObject*)npobj)->dob;

	// Silverlight always return 0.
	if (name == NPID ("count")) {
		INT32_TO_NPVARIANT (col->list->Length(), *result);	  
		return true;
	}

	return false;
}

bool
MoonlightCollectionClass::HasMethod (MoonlightObject *npobj, NPIdentifier name)
{
  DEBUGMSG ("MoonlightCollectionClass::HasMethod");
	if (HAS_METHOD (methods, name))
		return true;

	return MoonlightDependencyObjectClass::HasMethod (npobj, name);
}

bool
MoonlightCollectionClass::Invoke (MoonlightObject *npobj, NPIdentifier name,
					const NPVariant *args, uint32_t argCount,
					NPVariant *result)
{
  DEBUGMSG ("MoonlightCollectionClass::Invoke");
	Collection *col = (Collection*)((MoonlightDependencyObjectObject*)npobj)->dob;

	if (name == NPID ("add")) {
		if (argCount < 1)
			return true;

		MoonlightDependencyObjectObject *el = (MoonlightDependencyObjectObject*)NPVARIANT_TO_OBJECT (args[0]);
		col->Add (el->dob);

		VOID_TO_NPVARIANT (*result);

		return true;
	}
	else if (name == NPID ("remove")) {
		if (argCount < 1)
			return true;

		MoonlightDependencyObjectObject *el = (MoonlightDependencyObjectObject*)NPVARIANT_TO_OBJECT (args[0]);
		col->Remove (el->dob);

		VOID_TO_NPVARIANT (*result);

		return true;
	}
	else if (name == NPID ("insert")) {
		if (argCount < 2)
			return true;

		int index = NPVARIANT_TO_INT32 (args[0]);
		MoonlightDependencyObjectObject *el = (MoonlightDependencyObjectObject*)NPVARIANT_TO_OBJECT (args[1]);

		col->Insert (index, el->dob);

		VOID_TO_NPVARIANT (*result);

		return true;
	}
	else if (name == NPID ("clear")) {
		if (argCount != 0)
		  return true;

		col->Clear ();

		VOID_TO_NPVARIANT (*result);

		return true;
	}
	else
		return MoonlightDependencyObjectClass::Invoke (npobj, name,
							       args, argCount,
							       result);
}


/*** MoonlightStoryboardClass ***************************************************/

MoonlightStoryboardClass* MoonlightStoryboardClass::_class = NULL;

const char *const MoonlightStoryboardClass::methods [] = {
	"begin",
	"pause",
	"resume",
	"seek",
	"stop"
};

MoonlightStoryboardClass::MoonlightStoryboardClass ()
	: MoonlightDependencyObjectClass ()
{
}

bool
MoonlightStoryboardClass::HasMethod (MoonlightObject *npobj, NPIdentifier name)
{
  DEBUGMSG ("MoonlightStoryboardClass::HasMethod");
	if (HAS_METHOD (methods, name))
		return true;

	return MoonlightDependencyObjectClass::HasMethod (npobj, name);
}

bool
MoonlightStoryboardClass::Invoke (MoonlightObject *npobj, NPIdentifier name,
				  const NPVariant *args, uint32_t argCount,
				  NPVariant *result)
{
  DEBUGMSG ("MoonlightStoryboardClass::Invoke");
	Storyboard *sb = (Storyboard*)((MoonlightDependencyObjectObject*)npobj)->dob;

	if (name == NPID ("begin")) {
		if (argCount != 0)
			return true;

		sb->Begin ();

		VOID_TO_NPVARIANT (*result);

		return true;
	}
	else if (name == NPID ("pause")) {
		if (argCount != 0)
			return true;

		sb->Pause ();

		VOID_TO_NPVARIANT (*result);

		return true;
	}
	else if (name == NPID ("resume")) {
		if (argCount != 0)
			return true;

		sb->Resume ();

		VOID_TO_NPVARIANT (*result);

		return true;
	}
	else if (name == NPID ("seek")) {
		// not yet implemented
#if notyet
		if (argCount != 1)
			return true;

		sb->Seek (...);

		VOID_TO_NPVARIANT (*result);
#endif
		return true;
	}
	else if (name == NPID ("stop")) {
		if (argCount != 0)
			return true;

		sb->Stop ();

		VOID_TO_NPVARIANT (*result);

		return true;
	}
	else
		return MoonlightDependencyObjectClass::Invoke (npobj, name,
							       args, argCount,
							       result);
}

