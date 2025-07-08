#include "register_types.h"

#include "full_screen_control.h"

#include "mobile_button.h"
#include "stylebox_mobile_button.h"
#include "texture_mobile_button.h"

#include "animated_grid_container.h"

#include "safe_area_container.h"
#include "safe_area_expand.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(FullScreenControl);

	GDREGISTER_CLASS(MobileButton);
	GDREGISTER_CLASS(MobileButtonGroup);
	GDREGISTER_CLASS(StyleboxMobileButton);
	GDREGISTER_CLASS(TextureMobileButton);

	GDREGISTER_CLASS(AnimatedGridContainer);

	GDREGISTER_CLASS(SafeAreaContainer);
	GDREGISTER_CLASS(SafeAreaExpand);
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT GodControls_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
