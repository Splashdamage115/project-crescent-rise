



















#ifndef UFBX_UFBX_H_INCLUDED
#define UFBX_UFBX_H_INCLUDED



#if defined(UFBX_CONFIG_HEADER)
	#include UFBX_CONFIG_HEADER
#endif



#if !defined(UFBX_NO_LIBC_TYPES)
	#include <stdint.h>
	#include <stddef.h>
	#include <stdbool.h>
#endif



#ifndef UFBX_STDC
	#if defined(__STDC_VERSION__)
		#define UFBX_STDC __STDC_VERSION__
	#else
		#define UFBX_STDC 0
	#endif
#endif

#ifndef UFBX_CPP
	#if defined(__cplusplus)
		#define UFBX_CPP __cplusplus
	#else
		#define UFBX_CPP 0
	#endif
#endif

#ifndef UFBX_PLATFORM_MSC
	#if !defined(UFBX_STANDARD_C) && defined(_MSC_VER)
		#define UFBX_PLATFORM_MSC _MSC_VER
	#else
		#define UFBX_PLATFORM_MSC 0
	#endif
#endif

#ifndef UFBX_PLATFORM_GNUC
	#if !defined(UFBX_STANDARD_C) && defined(__GNUC__)
		#define UFBX_PLATFORM_GNUC __GNUC__
	#else
		#define UFBX_PLATFORM_GNUC 0
	#endif
#endif

#ifndef UFBX_CPP11
	
	#if UFBX_CPP >= 201103L || (UFBX_CPP > 0 && UFBX_PLATFORM_MSC >= 1900)
		#define UFBX_CPP11 1
	#else
		#define UFBX_CPP11 0
	#endif
#endif

#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable: 4061) 
	#pragma warning(disable: 4201) 
	#pragma warning(disable: 4505) 
	#pragma warning(disable: 4820) 
#elif defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wpedantic"
	#pragma clang diagnostic ignored "-Wpadded"
	#if defined(__cplusplus)
		#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
		#pragma clang diagnostic ignored "-Wold-style-cast"
	#endif
#elif defined(__GNUC__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	#pragma GCC diagnostic ignored "-Wpadded"
	#if defined(__cplusplus)
		#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
		#pragma GCC diagnostic ignored "-Wold-style-cast"
	#else
		#if __GNUC__ >= 5
			#pragma GCC diagnostic ignored "-Wc90-c99-compat"
			#pragma GCC diagnostic ignored "-Wc99-c11-compat"
		#endif
	#endif
#endif

#if UFBX_PLATFORM_MSC
	#define ufbx_inline static __forceinline
#elif UFBX_PLATFORM_GNUC
	#define ufbx_inline static inline __attribute__((always_inline, unused))
#else
	#define ufbx_inline static
#endif





#ifndef ufbx_assert
	#if defined(UFBX_NO_ASSERT) || defined(UFBX_NO_LIBC)
		#define ufbx_assert(cond) (void)0
	#else
		#include <assert.h>
		#define ufbx_assert(cond) assert(cond)
	#endif
#endif


#define ufbx_nullable



#define ufbx_unsafe







#ifndef ufbx_abi
	#if defined(UFBX_STATIC)
		#define ufbx_abi static
	#else
		#define ufbx_abi
	#endif
#endif



#ifndef ufbx_abi_data
	#if defined(UFBX_STATIC)
		#define ufbx_abi_data static
	#else
		#define ufbx_abi_data extern
	#endif
#endif



#ifndef ufbx_abi_data_definition
	#if defined(UFBX_STATIC)
		#define ufbx_abi_data_def static
	#else
		#define ufbx_abi_data_def
	#endif
#endif



#ifndef UFBX_REAL_TYPE
	#if defined(UFBX_REAL_IS_FLOAT)
		#define UFBX_REAL_TYPE float
	#else
		#define UFBX_REAL_TYPE double
	#endif
#endif


#define UFBX_ERROR_STACK_MAX_DEPTH 8
#define UFBX_PANIC_MESSAGE_LENGTH 128
#define UFBX_ERROR_INFO_LENGTH 256




#define UFBX_THREAD_GROUP_COUNT 4





#if UFBX_CPP11

template <typename T, typename U>
struct ufbxi_type_is { };

template <typename T>
struct ufbxi_type_is<T, T> { using type = int; };

template <typename T>
struct ufbx_converter { };

#define UFBX_CONVERSION_IMPL(p_name) \
	template <typename T, typename S=typename ufbxi_type_is<T, decltype(ufbx_converter<T>::from(*(const p_name*)nullptr))>::type> \
	operator T() const { return ufbx_converter<T>::from(*this); }

#define UFBX_CONVERSION_TO_IMPL(p_name) \
	template <typename T, typename S=typename ufbxi_type_is<p_name, decltype(ufbx_converter<T>::to(*(const T*)nullptr))>::type> \
	p_name(const T &t) { *this = ufbx_converter<T>::to(t); }

#define UFBX_CONVERSION_LIST_IMPL(p_name) \
	template <typename T, typename S=typename ufbxi_type_is<T, decltype(ufbx_converter<T>::from_list((p_name*)nullptr, (size_t)0))>::type> \
	operator T() const { return ufbx_converter<T>::from_list(data, count); }

#else

#define UFBX_CONVERSION_IMPL(p_name)
#define UFBX_CONVERSION_TO_IMPL(p_name)
#define UFBX_CONVERSION_LIST_IMPL(p_name)

#endif

#if defined(__cplusplus)
	#define UFBX_LIST_TYPE(p_name, p_type) struct p_name { p_type *data; size_t count; \
		p_type &operator[](size_t index) const { ufbx_assert(index < count); return data[index]; } \
		p_type *begin() const { return data; } \
		p_type *end() const { return data + count; } \
		UFBX_CONVERSION_LIST_IMPL(p_type) \
	}
#else
	#define UFBX_LIST_TYPE(p_name, p_type) typedef struct p_name { p_type *data; size_t count; } p_name
#endif




#if (UFBX_STDC >= 202311L || UFBX_CPP11) && defined(UFBX_USE_EXPLICIT_ENUM)
	#define UFBX_ENUM_REPR : int
	#define UFBX_ENUM_FORCE_WIDTH(p_prefix)
	#define UFBX_FLAG_REPR : int
	#define UFBX_FLAG_FORCE_WIDTH(p_prefix)
	#define UFBX_HAS_FORCE_32BIT 0
#else
	#define UFBX_ENUM_REPR
	#define UFBX_ENUM_FORCE_WIDTH(p_prefix) p_prefix##_FORCE_32BIT = 0x7fffffff
	#define UFBX_FLAG_REPR
	#define UFBX_FLAG_FORCE_WIDTH(p_prefix) p_prefix##_FORCE_32BIT = 0x7fffffff
	#define UFBX_HAS_FORCE_32BIT 1
#endif

#define UFBX_ENUM_TYPE(p_name, p_prefix, p_last) \
	enum { p_prefix##_COUNT = p_last + 1 }

#if UFBX_CPP
	#define UFBX_VERTEX_ATTRIB_IMPL(p_type) \
		p_type &operator[](size_t index) const { ufbx_assert(index < indices.count); return values.data[indices.data[index]]; }
#else
	#define UFBX_VERTEX_ATTRIB_IMPL(p_type)
#endif

#if UFBX_CPP11
	#define UFBX_CALLBACK_IMPL(p_name, p_fn, p_return, p_params, p_args) \
		template <typename F> static p_return _cpp_adapter p_params { F &f = *static_cast<F*>(user); return f p_args; } \
		p_name() = default; \
		p_name(p_fn *f) : fn(f), user(nullptr) { } \
		template <typename F> p_name(F *f) : fn(&_cpp_adapter<F>), user(static_cast<void*>(f)) { }
#else
	#define UFBX_CALLBACK_IMPL(p_name, p_fn, p_return, p_params, p_args)
#endif






#define ufbx_pack_version(major, minor, patch) ((major)*1000000u + (minor)*1000u + (patch))
#define ufbx_version_major(version) ((uint32_t)(version)/1000000u%1000u)
#define ufbx_version_minor(version) ((uint32_t)(version)/1000u%1000u)
#define ufbx_version_patch(version) ((uint32_t)(version)%1000u)






#define UFBX_HEADER_VERSION ufbx_pack_version(0, 21, 2)
#define UFBX_VERSION UFBX_HEADER_VERSION







typedef UFBX_REAL_TYPE ufbx_real;


typedef struct ufbx_string {
	const char *data;
	size_t length;

	UFBX_CONVERSION_IMPL(ufbx_string)
} ufbx_string;


typedef struct ufbx_blob {
	const void *data;
	size_t size;

	UFBX_CONVERSION_IMPL(ufbx_blob)
} ufbx_blob;


typedef struct ufbx_vec2 {
	union {
		struct { ufbx_real x, y; };
		ufbx_real v[2];
	};

	UFBX_CONVERSION_IMPL(ufbx_vec2)
} ufbx_vec2;


typedef struct ufbx_vec3 {
	union {
		struct { ufbx_real x, y, z; };
		ufbx_real v[3];
	};

	UFBX_CONVERSION_IMPL(ufbx_vec3)
} ufbx_vec3;


typedef struct ufbx_vec4 {
	union {
		struct { ufbx_real x, y, z, w; };
		ufbx_real v[4];
	};

	UFBX_CONVERSION_IMPL(ufbx_vec4)
} ufbx_vec4;


typedef struct ufbx_quat {
	union {
		struct { ufbx_real x, y, z, w; };
		ufbx_real v[4];
	};

	UFBX_CONVERSION_IMPL(ufbx_quat)
} ufbx_quat;





typedef enum ufbx_rotation_order UFBX_ENUM_REPR {
	UFBX_ROTATION_ORDER_XYZ,
	UFBX_ROTATION_ORDER_XZY,
	UFBX_ROTATION_ORDER_YZX,
	UFBX_ROTATION_ORDER_YXZ,
	UFBX_ROTATION_ORDER_ZXY,
	UFBX_ROTATION_ORDER_ZYX,
	UFBX_ROTATION_ORDER_SPHERIC,

	UFBX_ENUM_FORCE_WIDTH(UFBX_ROTATION_ORDER)
} ufbx_rotation_order;

UFBX_ENUM_TYPE(ufbx_rotation_order, UFBX_ROTATION_ORDER, UFBX_ROTATION_ORDER_SPHERIC);



typedef struct ufbx_transform {
	ufbx_vec3 translation;
	ufbx_quat rotation;
	ufbx_vec3 scale;

	UFBX_CONVERSION_IMPL(ufbx_transform)
} ufbx_transform;



typedef struct ufbx_matrix {
	union {
		struct {
			ufbx_real m00, m10, m20;
			ufbx_real m01, m11, m21;
			ufbx_real m02, m12, m22;
			ufbx_real m03, m13, m23;
		};
		ufbx_vec3 cols[4];
		ufbx_real v[12];
	};

	UFBX_CONVERSION_IMPL(ufbx_matrix)
} ufbx_matrix;

typedef struct ufbx_void_list {
	void *data;
	size_t count;
} ufbx_void_list;

UFBX_LIST_TYPE(ufbx_bool_list, bool);
UFBX_LIST_TYPE(ufbx_uint32_list, uint32_t);
UFBX_LIST_TYPE(ufbx_real_list, ufbx_real);
UFBX_LIST_TYPE(ufbx_vec2_list, ufbx_vec2);
UFBX_LIST_TYPE(ufbx_vec3_list, ufbx_vec3);
UFBX_LIST_TYPE(ufbx_vec4_list, ufbx_vec4);
UFBX_LIST_TYPE(ufbx_string_list, ufbx_string);


#define UFBX_NO_INDEX ((uint32_t)~0u)



typedef enum ufbx_dom_value_type UFBX_ENUM_REPR {
	UFBX_DOM_VALUE_NUMBER,
	UFBX_DOM_VALUE_STRING,
	UFBX_DOM_VALUE_BLOB,
	UFBX_DOM_VALUE_ARRAY_I32,
	UFBX_DOM_VALUE_ARRAY_I64,
	UFBX_DOM_VALUE_ARRAY_F32,
	UFBX_DOM_VALUE_ARRAY_F64,
	UFBX_DOM_VALUE_ARRAY_BLOB,
	UFBX_DOM_VALUE_ARRAY_IGNORED,

	UFBX_ENUM_FORCE_WIDTH(UFBX_DOM_VALUE_TYPE)
} ufbx_dom_value_type;

UFBX_ENUM_TYPE(ufbx_dom_value_type, UFBX_DOM_VALUE_TYPE, UFBX_DOM_VALUE_ARRAY_IGNORED);

typedef struct ufbx_dom_node ufbx_dom_node;

UFBX_LIST_TYPE(ufbx_int32_list, int32_t);
UFBX_LIST_TYPE(ufbx_int64_list, int64_t);
UFBX_LIST_TYPE(ufbx_float_list, float);
UFBX_LIST_TYPE(ufbx_double_list, double);
UFBX_LIST_TYPE(ufbx_blob_list, ufbx_blob);

typedef struct ufbx_dom_value {
	ufbx_dom_value_type type;
	ufbx_string value_str;
	ufbx_blob value_blob;
	int64_t value_int;
	double value_float;
} ufbx_dom_value;

UFBX_LIST_TYPE(ufbx_dom_node_list, ufbx_dom_node*);
UFBX_LIST_TYPE(ufbx_dom_value_list, ufbx_dom_value);

struct ufbx_dom_node {
	ufbx_string name;
	ufbx_dom_node_list children;
	ufbx_dom_value_list values;
};









typedef struct ufbx_prop ufbx_prop;
typedef struct ufbx_props ufbx_props;





typedef enum ufbx_prop_type UFBX_ENUM_REPR {
	UFBX_PROP_UNKNOWN,
	UFBX_PROP_BOOLEAN,
	UFBX_PROP_INTEGER,
	UFBX_PROP_NUMBER,
	UFBX_PROP_VECTOR,
	UFBX_PROP_COLOR,
	UFBX_PROP_COLOR_WITH_ALPHA,
	UFBX_PROP_STRING,
	UFBX_PROP_DATE_TIME,
	UFBX_PROP_TRANSLATION,
	UFBX_PROP_ROTATION,
	UFBX_PROP_SCALING,
	UFBX_PROP_DISTANCE,
	UFBX_PROP_COMPOUND,
	UFBX_PROP_BLOB,
	UFBX_PROP_REFERENCE,

	UFBX_ENUM_FORCE_WIDTH(UFBX_PROP_TYPE)
} ufbx_prop_type;

UFBX_ENUM_TYPE(ufbx_prop_type, UFBX_PROP_TYPE, UFBX_PROP_REFERENCE);


typedef enum ufbx_prop_flags UFBX_FLAG_REPR {
	
	
	UFBX_PROP_FLAG_ANIMATABLE = 0x1,

	
	UFBX_PROP_FLAG_USER_DEFINED = 0x2,

	
	UFBX_PROP_FLAG_HIDDEN = 0x4,

	
	UFBX_PROP_FLAG_LOCK_X = 0x10,
	UFBX_PROP_FLAG_LOCK_Y = 0x20,
	UFBX_PROP_FLAG_LOCK_Z = 0x40,
	UFBX_PROP_FLAG_LOCK_W = 0x80,

	
	UFBX_PROP_FLAG_MUTE_X = 0x100,
	UFBX_PROP_FLAG_MUTE_Y = 0x200,
	UFBX_PROP_FLAG_MUTE_Z = 0x400,
	UFBX_PROP_FLAG_MUTE_W = 0x800,

	
	
	
	UFBX_PROP_FLAG_SYNTHETIC = 0x1000,

	
	UFBX_PROP_FLAG_ANIMATED = 0x2000,

	
	UFBX_PROP_FLAG_NOT_FOUND = 0x4000,

	
	
	
	
	UFBX_PROP_FLAG_CONNECTED = 0x8000,

	
	UFBX_PROP_FLAG_NO_VALUE = 0x10000,

	
	
	UFBX_PROP_FLAG_OVERRIDDEN = 0x20000,

	
	
	
	UFBX_PROP_FLAG_VALUE_REAL = 0x100000,
	UFBX_PROP_FLAG_VALUE_VEC2 = 0x200000,
	UFBX_PROP_FLAG_VALUE_VEC3 = 0x400000,
	UFBX_PROP_FLAG_VALUE_VEC4 = 0x800000,
	UFBX_PROP_FLAG_VALUE_INT  = 0x1000000,
	UFBX_PROP_FLAG_VALUE_STR  = 0x2000000,
	UFBX_PROP_FLAG_VALUE_BLOB = 0x4000000,

	UFBX_FLAG_FORCE_WIDTH(UFBX_PROP_FLAGS)
} ufbx_prop_flags;


struct ufbx_prop {
	ufbx_string name;

	uint32_t _internal_key;

	ufbx_prop_type type;
	ufbx_prop_flags flags;

	ufbx_string value_str;
	ufbx_blob value_blob;
	int64_t value_int;
	union {
		ufbx_real value_real_arr[4];
		ufbx_real value_real;
		ufbx_vec2 value_vec2;
		ufbx_vec3 value_vec3;
		ufbx_vec4 value_vec4;
	};
};

UFBX_LIST_TYPE(ufbx_prop_list, ufbx_prop);




struct ufbx_props {
	ufbx_prop_list props;
	size_t num_animated;

	ufbx_nullable ufbx_props *defaults;
};

typedef struct ufbx_scene ufbx_scene;







typedef struct ufbx_element ufbx_element;


typedef struct ufbx_unknown ufbx_unknown;


typedef struct ufbx_node ufbx_node;


typedef struct ufbx_mesh ufbx_mesh;
typedef struct ufbx_light ufbx_light;
typedef struct ufbx_camera ufbx_camera;
typedef struct ufbx_bone ufbx_bone;
typedef struct ufbx_empty ufbx_empty;


typedef struct ufbx_line_curve ufbx_line_curve;
typedef struct ufbx_nurbs_curve ufbx_nurbs_curve;
typedef struct ufbx_nurbs_surface ufbx_nurbs_surface;
typedef struct ufbx_nurbs_trim_surface ufbx_nurbs_trim_surface;
typedef struct ufbx_nurbs_trim_boundary ufbx_nurbs_trim_boundary;


typedef struct ufbx_procedural_geometry ufbx_procedural_geometry;
typedef struct ufbx_stereo_camera ufbx_stereo_camera;
typedef struct ufbx_camera_switcher ufbx_camera_switcher;
typedef struct ufbx_marker ufbx_marker;
typedef struct ufbx_lod_group ufbx_lod_group;


typedef struct ufbx_skin_deformer ufbx_skin_deformer;
typedef struct ufbx_skin_cluster ufbx_skin_cluster;
typedef struct ufbx_blend_deformer ufbx_blend_deformer;
typedef struct ufbx_blend_channel ufbx_blend_channel;
typedef struct ufbx_blend_shape ufbx_blend_shape;
typedef struct ufbx_cache_deformer ufbx_cache_deformer;
typedef struct ufbx_cache_file ufbx_cache_file;


typedef struct ufbx_material ufbx_material;
typedef struct ufbx_texture ufbx_texture;
typedef struct ufbx_video ufbx_video;
typedef struct ufbx_shader ufbx_shader;
typedef struct ufbx_shader_binding ufbx_shader_binding;


typedef struct ufbx_anim_stack ufbx_anim_stack;
typedef struct ufbx_anim_layer ufbx_anim_layer;
typedef struct ufbx_anim_value ufbx_anim_value;
typedef struct ufbx_anim_curve ufbx_anim_curve;


typedef struct ufbx_display_layer ufbx_display_layer;
typedef struct ufbx_selection_set ufbx_selection_set;
typedef struct ufbx_selection_node ufbx_selection_node;


typedef struct ufbx_character ufbx_character;
typedef struct ufbx_constraint ufbx_constraint;


typedef struct ufbx_audio_layer ufbx_audio_layer;
typedef struct ufbx_audio_clip ufbx_audio_clip;


typedef struct ufbx_pose ufbx_pose;
typedef struct ufbx_metadata_object ufbx_metadata_object;

UFBX_LIST_TYPE(ufbx_element_list, ufbx_element*);
UFBX_LIST_TYPE(ufbx_unknown_list, ufbx_unknown*);
UFBX_LIST_TYPE(ufbx_node_list, ufbx_node*);
UFBX_LIST_TYPE(ufbx_mesh_list, ufbx_mesh*);
UFBX_LIST_TYPE(ufbx_light_list, ufbx_light*);
UFBX_LIST_TYPE(ufbx_camera_list, ufbx_camera*);
UFBX_LIST_TYPE(ufbx_bone_list, ufbx_bone*);
UFBX_LIST_TYPE(ufbx_empty_list, ufbx_empty*);
UFBX_LIST_TYPE(ufbx_line_curve_list, ufbx_line_curve*);
UFBX_LIST_TYPE(ufbx_nurbs_curve_list, ufbx_nurbs_curve*);
UFBX_LIST_TYPE(ufbx_nurbs_surface_list, ufbx_nurbs_surface*);
UFBX_LIST_TYPE(ufbx_nurbs_trim_surface_list, ufbx_nurbs_trim_surface*);
UFBX_LIST_TYPE(ufbx_nurbs_trim_boundary_list, ufbx_nurbs_trim_boundary*);
UFBX_LIST_TYPE(ufbx_procedural_geometry_list, ufbx_procedural_geometry*);
UFBX_LIST_TYPE(ufbx_stereo_camera_list, ufbx_stereo_camera*);
UFBX_LIST_TYPE(ufbx_camera_switcher_list, ufbx_camera_switcher*);
UFBX_LIST_TYPE(ufbx_marker_list, ufbx_marker*);
UFBX_LIST_TYPE(ufbx_lod_group_list, ufbx_lod_group*);
UFBX_LIST_TYPE(ufbx_skin_deformer_list, ufbx_skin_deformer*);
UFBX_LIST_TYPE(ufbx_skin_cluster_list, ufbx_skin_cluster*);
UFBX_LIST_TYPE(ufbx_blend_deformer_list, ufbx_blend_deformer*);
UFBX_LIST_TYPE(ufbx_blend_channel_list, ufbx_blend_channel*);
UFBX_LIST_TYPE(ufbx_blend_shape_list, ufbx_blend_shape*);
UFBX_LIST_TYPE(ufbx_cache_deformer_list, ufbx_cache_deformer*);
UFBX_LIST_TYPE(ufbx_cache_file_list, ufbx_cache_file*);
UFBX_LIST_TYPE(ufbx_material_list, ufbx_material*);
UFBX_LIST_TYPE(ufbx_texture_list, ufbx_texture*);
UFBX_LIST_TYPE(ufbx_video_list, ufbx_video*);
UFBX_LIST_TYPE(ufbx_shader_list, ufbx_shader*);
UFBX_LIST_TYPE(ufbx_shader_binding_list, ufbx_shader_binding*);
UFBX_LIST_TYPE(ufbx_anim_stack_list, ufbx_anim_stack*);
UFBX_LIST_TYPE(ufbx_anim_layer_list, ufbx_anim_layer*);
UFBX_LIST_TYPE(ufbx_anim_value_list, ufbx_anim_value*);
UFBX_LIST_TYPE(ufbx_anim_curve_list, ufbx_anim_curve*);
UFBX_LIST_TYPE(ufbx_display_layer_list, ufbx_display_layer*);
UFBX_LIST_TYPE(ufbx_selection_set_list, ufbx_selection_set*);
UFBX_LIST_TYPE(ufbx_selection_node_list, ufbx_selection_node*);
UFBX_LIST_TYPE(ufbx_character_list, ufbx_character*);
UFBX_LIST_TYPE(ufbx_constraint_list, ufbx_constraint*);
UFBX_LIST_TYPE(ufbx_audio_layer_list, ufbx_audio_layer*);
UFBX_LIST_TYPE(ufbx_audio_clip_list, ufbx_audio_clip*);
UFBX_LIST_TYPE(ufbx_pose_list, ufbx_pose*);
UFBX_LIST_TYPE(ufbx_metadata_object_list, ufbx_metadata_object*);

typedef enum ufbx_element_type UFBX_ENUM_REPR {
	UFBX_ELEMENT_UNKNOWN,             
	UFBX_ELEMENT_NODE,                
	UFBX_ELEMENT_MESH,                
	UFBX_ELEMENT_LIGHT,               
	UFBX_ELEMENT_CAMERA,              
	UFBX_ELEMENT_BONE,                
	UFBX_ELEMENT_EMPTY,               
	UFBX_ELEMENT_LINE_CURVE,          
	UFBX_ELEMENT_NURBS_CURVE,         
	UFBX_ELEMENT_NURBS_SURFACE,       
	UFBX_ELEMENT_NURBS_TRIM_SURFACE,  
	UFBX_ELEMENT_NURBS_TRIM_BOUNDARY, 
	UFBX_ELEMENT_PROCEDURAL_GEOMETRY, 
	UFBX_ELEMENT_STEREO_CAMERA,       
	UFBX_ELEMENT_CAMERA_SWITCHER,     
	UFBX_ELEMENT_MARKER,              
	UFBX_ELEMENT_LOD_GROUP,           
	UFBX_ELEMENT_SKIN_DEFORMER,       
	UFBX_ELEMENT_SKIN_CLUSTER,        
	UFBX_ELEMENT_BLEND_DEFORMER,      
	UFBX_ELEMENT_BLEND_CHANNEL,       
	UFBX_ELEMENT_BLEND_SHAPE,         
	UFBX_ELEMENT_CACHE_DEFORMER,      
	UFBX_ELEMENT_CACHE_FILE,          
	UFBX_ELEMENT_MATERIAL,            
	UFBX_ELEMENT_TEXTURE,             
	UFBX_ELEMENT_VIDEO,               
	UFBX_ELEMENT_SHADER,              
	UFBX_ELEMENT_SHADER_BINDING,      
	UFBX_ELEMENT_ANIM_STACK,          
	UFBX_ELEMENT_ANIM_LAYER,          
	UFBX_ELEMENT_ANIM_VALUE,          
	UFBX_ELEMENT_ANIM_CURVE,          
	UFBX_ELEMENT_DISPLAY_LAYER,       
	UFBX_ELEMENT_SELECTION_SET,       
	UFBX_ELEMENT_SELECTION_NODE,      
	UFBX_ELEMENT_CHARACTER,           
	UFBX_ELEMENT_CONSTRAINT,          
	UFBX_ELEMENT_AUDIO_LAYER,         
	UFBX_ELEMENT_AUDIO_CLIP,          
	UFBX_ELEMENT_POSE,                
	UFBX_ELEMENT_METADATA_OBJECT,     

	UFBX_ELEMENT_TYPE_FIRST_ATTRIB = UFBX_ELEMENT_MESH,
	UFBX_ELEMENT_TYPE_LAST_ATTRIB = UFBX_ELEMENT_LOD_GROUP,

	UFBX_ENUM_FORCE_WIDTH(UFBX_ELEMENT_TYPE)
} ufbx_element_type;

UFBX_ENUM_TYPE(ufbx_element_type, UFBX_ELEMENT_TYPE, UFBX_ELEMENT_METADATA_OBJECT);




typedef struct ufbx_connection {
	ufbx_element *src;
	ufbx_element *dst;
	ufbx_string src_prop;
	ufbx_string dst_prop;
} ufbx_connection;

UFBX_LIST_TYPE(ufbx_connection_list, ufbx_connection);






struct ufbx_element {
	ufbx_string name;
	ufbx_props props;
	uint32_t element_id;
	uint32_t typed_id;
	ufbx_node_list instances;
	ufbx_element_type type;
	ufbx_connection_list connections_src;
	ufbx_connection_list connections_dst;
	ufbx_nullable ufbx_dom_node *dom_node;
	ufbx_scene *scene;
};



struct ufbx_unknown {
	
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	
	
	ufbx_string type;
	ufbx_string super_type;
	ufbx_string sub_type;
};








typedef enum ufbx_inherit_mode UFBX_ENUM_REPR {

	
	
	UFBX_INHERIT_MODE_NORMAL,

	
	
	
	
	
	
	UFBX_INHERIT_MODE_IGNORE_PARENT_SCALE,

	
	
	
	
	
	UFBX_INHERIT_MODE_COMPONENTWISE_SCALE,

	UFBX_ENUM_FORCE_WIDTH(UFBX_INHERIT_MODE)
} ufbx_inherit_mode;

UFBX_ENUM_TYPE(ufbx_inherit_mode, UFBX_INHERIT_MODE, UFBX_INHERIT_MODE_COMPONENTWISE_SCALE);


typedef enum ufbx_mirror_axis UFBX_ENUM_REPR {

	UFBX_MIRROR_AXIS_NONE,
	UFBX_MIRROR_AXIS_X,
	UFBX_MIRROR_AXIS_Y,
	UFBX_MIRROR_AXIS_Z,

	UFBX_ENUM_FORCE_WIDTH(UFBX_MIRROR_AXIS)
} ufbx_mirror_axis;

UFBX_ENUM_TYPE(ufbx_mirror_axis, UFBX_MIRROR_AXIS, UFBX_MIRROR_AXIS_Z);




struct ufbx_node {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	

	
	
	
	
	ufbx_nullable ufbx_node *parent;

	
	ufbx_node_list children;

	
	
	
	
	
	
	ufbx_nullable ufbx_mesh *mesh;
	ufbx_nullable ufbx_light *light;
	ufbx_nullable ufbx_camera *camera;
	ufbx_nullable ufbx_bone *bone;

	
	
	
	
	ufbx_nullable ufbx_element *attrib;

	
	
	ufbx_nullable ufbx_node *geometry_transform_helper;

	
	
	ufbx_nullable ufbx_node *scale_helper;

	
	ufbx_element_type attrib_type;

	
	
	
	
	ufbx_element_list all_attribs;

	
	
	ufbx_inherit_mode inherit_mode;
	ufbx_inherit_mode original_inherit_mode;
	ufbx_transform local_transform;
	ufbx_transform geometry_transform;

	
	
	ufbx_vec3 inherit_scale;

	
	
	
	
	
	ufbx_nullable ufbx_node *inherit_scale_node;

	

	
	ufbx_rotation_order rotation_order;
	
	
	ufbx_vec3 euler_rotation;

	
	

	
	
	ufbx_matrix node_to_parent;
	
	
	ufbx_matrix node_to_world;
	
	
	
	ufbx_matrix geometry_to_node;
	
	
	ufbx_matrix geometry_to_world;
	
	ufbx_matrix unscaled_node_to_world;

	
	
	
	ufbx_vec3 adjust_pre_translation;    
	ufbx_quat adjust_pre_rotation;       
	ufbx_real adjust_pre_scale;          
	ufbx_quat adjust_post_rotation;      
	ufbx_real adjust_post_scale;         
	ufbx_real adjust_translation_scale;  
	ufbx_mirror_axis adjust_mirror_axis; 

	
	
	
	ufbx_material_list materials;

	
	ufbx_nullable ufbx_pose *bind_pose;

	
	bool visible;

	
	bool is_root;

	
	bool has_geometry_transform;

	
	
	
	bool has_adjust_transform;

	
	bool has_root_adjust_transform;

	
	
	bool is_geometry_transform_helper;

	
	
	bool is_scale_helper;

	
	bool is_scale_compensate_parent;

	
	
	uint32_t node_depth;
};










typedef struct ufbx_vertex_attrib {
	
	bool exists;
	
	ufbx_void_list values;
	
	ufbx_uint32_list indices;
	
	size_t value_reals;
	
	bool unique_per_vertex;
	
	
	
	
	
	
	ufbx_real_list values_w;
} ufbx_vertex_attrib;


typedef struct ufbx_vertex_real {
	bool exists;
	ufbx_real_list values;
	ufbx_uint32_list indices;
	size_t value_reals;
	bool unique_per_vertex;
	ufbx_real_list values_w;

	UFBX_VERTEX_ATTRIB_IMPL(ufbx_real)
} ufbx_vertex_real;


typedef struct ufbx_vertex_vec2 {
	bool exists;
	ufbx_vec2_list values;
	ufbx_uint32_list indices;
	size_t value_reals;
	bool unique_per_vertex;
	ufbx_real_list values_w;

	UFBX_VERTEX_ATTRIB_IMPL(ufbx_vec2)
} ufbx_vertex_vec2;


typedef struct ufbx_vertex_vec3 {
	bool exists;
	ufbx_vec3_list values;
	ufbx_uint32_list indices;
	size_t value_reals;
	bool unique_per_vertex;
	ufbx_real_list values_w;

	UFBX_VERTEX_ATTRIB_IMPL(ufbx_vec3)
} ufbx_vertex_vec3;


typedef struct ufbx_vertex_vec4 {
	bool exists;
	ufbx_vec4_list values;
	ufbx_uint32_list indices;
	size_t value_reals;
	bool unique_per_vertex;
	ufbx_real_list values_w;

	UFBX_VERTEX_ATTRIB_IMPL(ufbx_vec4)
} ufbx_vertex_vec4;


typedef struct ufbx_uv_set {
	ufbx_string name;
	uint32_t index;

	
	ufbx_vertex_vec2 vertex_uv;        
	ufbx_vertex_vec3 vertex_tangent;   
	ufbx_vertex_vec3 vertex_bitangent; 
} ufbx_uv_set;


typedef struct ufbx_color_set {
	ufbx_string name;
	uint32_t index;

	
	ufbx_vertex_vec4 vertex_color; 
} ufbx_color_set;

UFBX_LIST_TYPE(ufbx_uv_set_list, ufbx_uv_set);
UFBX_LIST_TYPE(ufbx_color_set_list, ufbx_color_set);


typedef struct ufbx_edge {
	union {
		struct { uint32_t a, b; };
		uint32_t indices[2];
	};
} ufbx_edge;

UFBX_LIST_TYPE(ufbx_edge_list, ufbx_edge);






typedef struct ufbx_face {
	uint32_t index_begin;
	uint32_t num_indices;
} ufbx_face;

UFBX_LIST_TYPE(ufbx_face_list, ufbx_face);


typedef struct ufbx_mesh_part {

	
	uint32_t index;

	
	size_t num_faces;     
	size_t num_triangles; 

	size_t num_empty_faces; 
	size_t num_point_faces; 
	size_t num_line_faces;  

	
	
	ufbx_uint32_list face_indices;

} ufbx_mesh_part;

UFBX_LIST_TYPE(ufbx_mesh_part_list, ufbx_mesh_part);

typedef struct ufbx_face_group {
	int32_t id;       
	ufbx_string name; 
} ufbx_face_group;

UFBX_LIST_TYPE(ufbx_face_group_list, ufbx_face_group);

typedef struct ufbx_subdivision_weight_range {
	uint32_t weight_begin;
	uint32_t num_weights;
} ufbx_subdivision_weight_range;

UFBX_LIST_TYPE(ufbx_subdivision_weight_range_list, ufbx_subdivision_weight_range);

typedef struct ufbx_subdivision_weight {
	ufbx_real weight;
	uint32_t index;
} ufbx_subdivision_weight;

UFBX_LIST_TYPE(ufbx_subdivision_weight_list, ufbx_subdivision_weight);

typedef struct ufbx_subdivision_result {
	size_t result_memory_used;
	size_t temp_memory_used;
	size_t result_allocs;
	size_t temp_allocs;

	
	
	ufbx_subdivision_weight_range_list source_vertex_ranges;
	ufbx_subdivision_weight_list source_vertex_weights;

	
	
	ufbx_subdivision_weight_range_list skin_cluster_ranges;
	ufbx_subdivision_weight_list skin_cluster_weights;

} ufbx_subdivision_result;

typedef enum ufbx_subdivision_display_mode UFBX_ENUM_REPR {
	UFBX_SUBDIVISION_DISPLAY_DISABLED,
	UFBX_SUBDIVISION_DISPLAY_HULL,
	UFBX_SUBDIVISION_DISPLAY_HULL_AND_SMOOTH,
	UFBX_SUBDIVISION_DISPLAY_SMOOTH,

	UFBX_ENUM_FORCE_WIDTH(UFBX_SUBDIVISION_DISPLAY_MODE)
} ufbx_subdivision_display_mode;

UFBX_ENUM_TYPE(ufbx_subdivision_display_mode, UFBX_SUBDIVISION_DISPLAY_MODE, UFBX_SUBDIVISION_DISPLAY_SMOOTH);

typedef enum ufbx_subdivision_boundary UFBX_ENUM_REPR {
	UFBX_SUBDIVISION_BOUNDARY_DEFAULT,
	UFBX_SUBDIVISION_BOUNDARY_LEGACY,
	
	UFBX_SUBDIVISION_BOUNDARY_SHARP_CORNERS,
	
	UFBX_SUBDIVISION_BOUNDARY_SHARP_NONE,
	
	UFBX_SUBDIVISION_BOUNDARY_SHARP_BOUNDARY,
	
	UFBX_SUBDIVISION_BOUNDARY_SHARP_INTERIOR,

	UFBX_ENUM_FORCE_WIDTH(UFBX_SUBDIVISION_BOUNDARY)
} ufbx_subdivision_boundary;

UFBX_ENUM_TYPE(ufbx_subdivision_boundary, UFBX_SUBDIVISION_BOUNDARY, UFBX_SUBDIVISION_BOUNDARY_SHARP_INTERIOR);

















































struct ufbx_mesh {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };

	
	
	size_t num_vertices;  
	size_t num_indices;   
	size_t num_faces;     
	size_t num_triangles; 

	
	
	size_t num_edges;

	size_t max_face_triangles; 

	size_t num_empty_faces; 
	size_t num_point_faces; 
	size_t num_line_faces;  

	
	ufbx_face_list faces;           
	ufbx_bool_list face_smoothing;  
	ufbx_uint32_list face_material; 
	ufbx_uint32_list face_group;    
	ufbx_bool_list face_hole;       

	
	ufbx_edge_list edges;           
	ufbx_bool_list edge_smoothing;  
	ufbx_real_list edge_crease;     
	ufbx_bool_list edge_visibility; 

	
	
	ufbx_uint32_list vertex_indices;
	ufbx_vec3_list vertices;

	
	ufbx_uint32_list vertex_first_index;

	
	
	
	
	
	
	ufbx_vertex_vec3 vertex_position;  
	ufbx_vertex_vec3 vertex_normal;    
	ufbx_vertex_vec2 vertex_uv;        
	ufbx_vertex_vec3 vertex_tangent;   
	ufbx_vertex_vec3 vertex_bitangent; 
	ufbx_vertex_vec4 vertex_color;     
	ufbx_vertex_real vertex_crease;    

	
	
	ufbx_uv_set_list uv_sets;
	ufbx_color_set_list color_sets;

	
	
	
	ufbx_material_list materials;

	
	ufbx_face_group_list face_groups;

	
	
	ufbx_mesh_part_list material_parts;

	
	ufbx_mesh_part_list face_group_parts;

	
	
	
	ufbx_uint32_list material_part_usage_order;

	
	
	
	
	bool skinned_is_local;
	ufbx_vertex_vec3 skinned_position;
	ufbx_vertex_vec3 skinned_normal;

	
	ufbx_skin_deformer_list skin_deformers;
	ufbx_blend_deformer_list blend_deformers;
	ufbx_cache_deformer_list cache_deformers;
	ufbx_element_list all_deformers;

	
	uint32_t subdivision_preview_levels;
	uint32_t subdivision_render_levels;
	ufbx_subdivision_display_mode subdivision_display_mode;
	ufbx_subdivision_boundary subdivision_boundary;
	ufbx_subdivision_boundary subdivision_uv_boundary;

	
	bool reversed_winding;

	
	
	
	bool generated_normals;

	
	bool subdivision_evaluated;
	ufbx_nullable ufbx_subdivision_result *subdivision_result;

	
	bool from_tessellated_nurbs;
};


typedef enum ufbx_light_type UFBX_ENUM_REPR {
	
	UFBX_LIGHT_POINT,
	
	
	UFBX_LIGHT_DIRECTIONAL,
	
	
	UFBX_LIGHT_SPOT,
	
	
	UFBX_LIGHT_AREA,
	
	
	UFBX_LIGHT_VOLUME,

	UFBX_ENUM_FORCE_WIDTH(UFBX_LIGHT_TYPE)
} ufbx_light_type;

UFBX_ENUM_TYPE(ufbx_light_type, UFBX_LIGHT_TYPE, UFBX_LIGHT_VOLUME);


typedef enum ufbx_light_decay UFBX_ENUM_REPR {
	UFBX_LIGHT_DECAY_NONE,      
	UFBX_LIGHT_DECAY_LINEAR,    
	UFBX_LIGHT_DECAY_QUADRATIC, 
	UFBX_LIGHT_DECAY_CUBIC,     

	UFBX_ENUM_FORCE_WIDTH(UFBX_LIGHT_DECAY)
} ufbx_light_decay;

UFBX_ENUM_TYPE(ufbx_light_decay, UFBX_LIGHT_DECAY, UFBX_LIGHT_DECAY_CUBIC);

typedef enum ufbx_light_area_shape UFBX_ENUM_REPR {
	UFBX_LIGHT_AREA_SHAPE_RECTANGLE,
	UFBX_LIGHT_AREA_SHAPE_SPHERE,

	UFBX_ENUM_FORCE_WIDTH(UFBX_LIGHT_AREA_SHAPE)
} ufbx_light_area_shape;

UFBX_ENUM_TYPE(ufbx_light_area_shape, UFBX_LIGHT_AREA_SHAPE, UFBX_LIGHT_AREA_SHAPE_SPHERE);


struct ufbx_light {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };

	
	
	
	ufbx_vec3 color;
	ufbx_real intensity;

	
	ufbx_vec3 local_direction;

	
	ufbx_light_type type;
	ufbx_light_decay decay;
	ufbx_light_area_shape area_shape;
	ufbx_real inner_angle;
	ufbx_real outer_angle;

	bool cast_light;
	bool cast_shadows;
};

typedef enum ufbx_projection_mode UFBX_ENUM_REPR {
	
	UFBX_PROJECTION_MODE_PERSPECTIVE,

	
	UFBX_PROJECTION_MODE_ORTHOGRAPHIC,

	UFBX_ENUM_FORCE_WIDTH(UFBX_PROJECTION_MODE)
} ufbx_projection_mode;

UFBX_ENUM_TYPE(ufbx_projection_mode, UFBX_PROJECTION_MODE, UFBX_PROJECTION_MODE_ORTHOGRAPHIC);



typedef enum ufbx_aspect_mode UFBX_ENUM_REPR {
	
	UFBX_ASPECT_MODE_WINDOW_SIZE,
	
	UFBX_ASPECT_MODE_FIXED_RATIO,
	
	UFBX_ASPECT_MODE_FIXED_RESOLUTION,
	
	UFBX_ASPECT_MODE_FIXED_WIDTH,
	
	UFBX_ASPECT_MODE_FIXED_HEIGHT,

	UFBX_ENUM_FORCE_WIDTH(UFBX_ASPECT_MODE)
} ufbx_aspect_mode;

UFBX_ENUM_TYPE(ufbx_aspect_mode, UFBX_ASPECT_MODE, UFBX_ASPECT_MODE_FIXED_HEIGHT);



typedef enum ufbx_aperture_mode UFBX_ENUM_REPR {
	
	UFBX_APERTURE_MODE_HORIZONTAL_AND_VERTICAL,
	
	UFBX_APERTURE_MODE_HORIZONTAL,
	
	UFBX_APERTURE_MODE_VERTICAL,
	
	UFBX_APERTURE_MODE_FOCAL_LENGTH,

	UFBX_ENUM_FORCE_WIDTH(UFBX_APERTURE_MODE)
} ufbx_aperture_mode;

UFBX_ENUM_TYPE(ufbx_aperture_mode, UFBX_APERTURE_MODE, UFBX_APERTURE_MODE_FOCAL_LENGTH);



typedef enum ufbx_gate_fit UFBX_ENUM_REPR {
	
	UFBX_GATE_FIT_NONE,
	
	UFBX_GATE_FIT_VERTICAL,
	
	UFBX_GATE_FIT_HORIZONTAL,
	
	UFBX_GATE_FIT_FILL,
	
	UFBX_GATE_FIT_OVERSCAN,
	
	
	UFBX_GATE_FIT_STRETCH,

	UFBX_ENUM_FORCE_WIDTH(UFBX_GATE_FIT)
} ufbx_gate_fit;

UFBX_ENUM_TYPE(ufbx_gate_fit, UFBX_GATE_FIT, UFBX_GATE_FIT_STRETCH);



typedef enum ufbx_aperture_format UFBX_ENUM_REPR {
	UFBX_APERTURE_FORMAT_CUSTOM,              
	UFBX_APERTURE_FORMAT_16MM_THEATRICAL,     
	UFBX_APERTURE_FORMAT_SUPER_16MM,          
	UFBX_APERTURE_FORMAT_35MM_ACADEMY,        
	UFBX_APERTURE_FORMAT_35MM_TV_PROJECTION,  
	UFBX_APERTURE_FORMAT_35MM_FULL_APERTURE,  
	UFBX_APERTURE_FORMAT_35MM_185_PROJECTION, 
	UFBX_APERTURE_FORMAT_35MM_ANAMORPHIC,     
	UFBX_APERTURE_FORMAT_70MM_PROJECTION,     
	UFBX_APERTURE_FORMAT_VISTAVISION,         
	UFBX_APERTURE_FORMAT_DYNAVISION,          
	UFBX_APERTURE_FORMAT_IMAX,                

	UFBX_ENUM_FORCE_WIDTH(UFBX_APERTURE_FORMAT)
} ufbx_aperture_format;

UFBX_ENUM_TYPE(ufbx_aperture_format, UFBX_APERTURE_FORMAT, UFBX_APERTURE_FORMAT_IMAX);

typedef enum ufbx_coordinate_axis UFBX_ENUM_REPR {
	UFBX_COORDINATE_AXIS_POSITIVE_X,
	UFBX_COORDINATE_AXIS_NEGATIVE_X,
	UFBX_COORDINATE_AXIS_POSITIVE_Y,
	UFBX_COORDINATE_AXIS_NEGATIVE_Y,
	UFBX_COORDINATE_AXIS_POSITIVE_Z,
	UFBX_COORDINATE_AXIS_NEGATIVE_Z,
	UFBX_COORDINATE_AXIS_UNKNOWN,

	UFBX_ENUM_FORCE_WIDTH(UFBX_COORDINATE_AXIS)
} ufbx_coordinate_axis;

UFBX_ENUM_TYPE(ufbx_coordinate_axis, UFBX_COORDINATE_AXIS, UFBX_COORDINATE_AXIS_UNKNOWN);



typedef struct ufbx_coordinate_axes {
	ufbx_coordinate_axis right;
	ufbx_coordinate_axis up;
	ufbx_coordinate_axis front;
} ufbx_coordinate_axes;


struct ufbx_camera {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };

	
	ufbx_projection_mode projection_mode;

	
	
	bool resolution_is_pixels;

	
	ufbx_vec2 resolution;

	
	
	ufbx_vec2 field_of_view_deg;

	
	
	
	ufbx_vec2 field_of_view_tan;

	
	
	ufbx_real orthographic_extent;

	
	
	ufbx_vec2 orthographic_size;

	
	
	ufbx_vec2 projection_plane;

	
	ufbx_real aspect_ratio;

	
	ufbx_real near_plane;

	
	ufbx_real far_plane;

	
	
	
	ufbx_coordinate_axes projection_axes;

	
	ufbx_aspect_mode aspect_mode;
	ufbx_aperture_mode aperture_mode;
	ufbx_gate_fit gate_fit;
	ufbx_aperture_format aperture_format;
	ufbx_real focal_length_mm;     
	ufbx_vec2 film_size_inch;      
	ufbx_vec2 aperture_size_inch;  
	ufbx_real squeeze_ratio;       
};



struct ufbx_bone {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };

	
	ufbx_real radius;

	
	ufbx_real relative_length;

	
	bool is_root;
};


struct ufbx_empty {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };
};




typedef struct ufbx_line_segment {
	uint32_t index_begin;
	uint32_t num_indices;
} ufbx_line_segment;

UFBX_LIST_TYPE(ufbx_line_segment_list, ufbx_line_segment);

struct ufbx_line_curve {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };

	ufbx_vec3 color;

	ufbx_vec3_list control_points; 
	ufbx_uint32_list point_indices; 

	ufbx_line_segment_list segments;

	
	bool from_tessellated_nurbs;
};

typedef enum ufbx_nurbs_topology UFBX_ENUM_REPR {
	
	UFBX_NURBS_TOPOLOGY_OPEN,
	
	UFBX_NURBS_TOPOLOGY_PERIODIC,
	
	UFBX_NURBS_TOPOLOGY_CLOSED,

	UFBX_ENUM_FORCE_WIDTH(UFBX_NURBS_TOPOLOGY)
} ufbx_nurbs_topology;

UFBX_ENUM_TYPE(ufbx_nurbs_topology, UFBX_NURBS_TOPOLOGY, UFBX_NURBS_TOPOLOGY_CLOSED);


typedef struct ufbx_nurbs_basis {

	
	
	uint32_t order;

	
	ufbx_nurbs_topology topology;

	
	ufbx_real_list knot_vector;

	
	ufbx_real t_min;
	ufbx_real t_max;

	
	ufbx_real_list spans;

	
	bool is_2d;

	
	
	
	
	
	
	size_t num_wrap_control_points;

	
	bool valid;

} ufbx_nurbs_basis;

struct ufbx_nurbs_curve {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };

	
	ufbx_nurbs_basis basis;

	
	
	
	ufbx_vec4_list control_points;
};

struct ufbx_nurbs_surface {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };

	
	ufbx_nurbs_basis basis_u;
	ufbx_nurbs_basis basis_v;

	
	size_t num_control_points_u;
	size_t num_control_points_v;

	
	
	
	
	ufbx_vec4_list control_points;

	
	uint32_t span_subdivision_u;
	uint32_t span_subdivision_v;

	
	bool flip_normals;

	
	
	ufbx_nullable ufbx_material *material;
};

struct ufbx_nurbs_trim_surface {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };
};

struct ufbx_nurbs_trim_boundary {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };
};



struct ufbx_procedural_geometry {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };
};

struct ufbx_stereo_camera {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };

	ufbx_nullable ufbx_camera *left;
	ufbx_nullable ufbx_camera *right;
};

struct ufbx_camera_switcher {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };
};

typedef enum ufbx_marker_type UFBX_ENUM_REPR {
	UFBX_MARKER_UNKNOWN,     
	UFBX_MARKER_FK_EFFECTOR, 
	UFBX_MARKER_IK_EFFECTOR, 

	UFBX_ENUM_FORCE_WIDTH(UFBX_MARKER_TYPE)
} ufbx_marker_type;

UFBX_ENUM_TYPE(ufbx_marker_type, UFBX_MARKER_TYPE, UFBX_MARKER_IK_EFFECTOR);


struct ufbx_marker {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };

	
	ufbx_marker_type type;
};


typedef enum ufbx_lod_display UFBX_ENUM_REPR {
	UFBX_LOD_DISPLAY_USE_LOD, 
	UFBX_LOD_DISPLAY_SHOW,    
	UFBX_LOD_DISPLAY_HIDE,    

	UFBX_ENUM_FORCE_WIDTH(UFBX_LOD_DISPLAY)
} ufbx_lod_display;

UFBX_ENUM_TYPE(ufbx_lod_display, UFBX_LOD_DISPLAY, UFBX_LOD_DISPLAY_HIDE);



typedef struct ufbx_lod_level {

	
	
	
	ufbx_real distance;

	
	
	
	ufbx_lod_display display;

} ufbx_lod_level;

UFBX_LIST_TYPE(ufbx_lod_level_list, ufbx_lod_level);



struct ufbx_lod_group {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
		ufbx_node_list instances;
	}; };

	
	bool relative_distances;

	
	ufbx_lod_level_list lod_levels;

	
	bool ignore_parent_transform;

	
	
	bool use_distance_limit;
	ufbx_real distance_limit_min;
	ufbx_real distance_limit_max;
};




typedef enum ufbx_skinning_method UFBX_ENUM_REPR {
	
	UFBX_SKINNING_METHOD_LINEAR,
	
	UFBX_SKINNING_METHOD_RIGID,
	
	UFBX_SKINNING_METHOD_DUAL_QUATERNION,
	
	
	
	UFBX_SKINNING_METHOD_BLENDED_DQ_LINEAR,

	UFBX_ENUM_FORCE_WIDTH(UFBX_SKINNING_METHOD)
} ufbx_skinning_method;

UFBX_ENUM_TYPE(ufbx_skinning_method, UFBX_SKINNING_METHOD, UFBX_SKINNING_METHOD_BLENDED_DQ_LINEAR);


typedef struct ufbx_skin_vertex {

	
	
	
	
	uint32_t weight_begin; 
	uint32_t num_weights; 

	
	
	ufbx_real dq_weight;

} ufbx_skin_vertex;

UFBX_LIST_TYPE(ufbx_skin_vertex_list, ufbx_skin_vertex);


typedef struct ufbx_skin_weight {
	uint32_t cluster_index; 
	ufbx_real weight;       
} ufbx_skin_weight;

UFBX_LIST_TYPE(ufbx_skin_weight_list, ufbx_skin_weight);




struct ufbx_skin_deformer {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	ufbx_skinning_method skinning_method;

	
	ufbx_skin_cluster_list clusters;

	
	ufbx_skin_vertex_list vertices;
	ufbx_skin_weight_list weights;

	
	size_t max_weights_per_vertex;

	
	
	
	size_t num_dq_weights;
	ufbx_uint32_list dq_vertices;
	ufbx_real_list dq_weights;
};


struct ufbx_skin_cluster {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	
	
	ufbx_nullable ufbx_node *bone_node;

	
	ufbx_matrix geometry_to_bone;

	
	
	ufbx_matrix mesh_node_to_bone;

	
	
	ufbx_matrix bind_to_world;

	
	
	ufbx_matrix geometry_to_world;
	ufbx_transform geometry_to_world_transform;

	
	
	
	size_t num_weights;       
	ufbx_uint32_list vertices; 
	ufbx_real_list weights;   
};



struct ufbx_blend_deformer {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	ufbx_blend_channel_list channels;
};


typedef struct ufbx_blend_keyframe {
	
	ufbx_blend_shape *shape;

	
	ufbx_real target_weight;

	
	ufbx_real effective_weight;
} ufbx_blend_keyframe;

UFBX_LIST_TYPE(ufbx_blend_keyframe_list, ufbx_blend_keyframe);



struct ufbx_blend_channel {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	ufbx_real weight;

	
	
	ufbx_blend_keyframe_list keyframes;

	
	ufbx_nullable ufbx_blend_shape *target_shape;
};


struct ufbx_blend_shape {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	
	size_t num_offsets;               
	ufbx_uint32_list offset_vertices; 
	ufbx_vec3_list position_offsets;  
	ufbx_vec3_list normal_offsets;    

	
	
	ufbx_real_list offset_weights;
};

typedef enum ufbx_cache_file_format UFBX_ENUM_REPR {
	UFBX_CACHE_FILE_FORMAT_UNKNOWN, 
	UFBX_CACHE_FILE_FORMAT_PC2,     
	UFBX_CACHE_FILE_FORMAT_MC,      

	UFBX_ENUM_FORCE_WIDTH(UFBX_CACHE_FILE_FORMAT)
} ufbx_cache_file_format;

UFBX_ENUM_TYPE(ufbx_cache_file_format, UFBX_CACHE_FILE_FORMAT, UFBX_CACHE_FILE_FORMAT_MC);

typedef enum ufbx_cache_data_format UFBX_ENUM_REPR {
	UFBX_CACHE_DATA_FORMAT_UNKNOWN,     
	UFBX_CACHE_DATA_FORMAT_REAL_FLOAT,  
	UFBX_CACHE_DATA_FORMAT_VEC3_FLOAT,  
	UFBX_CACHE_DATA_FORMAT_REAL_DOUBLE, 
	UFBX_CACHE_DATA_FORMAT_VEC3_DOUBLE, 

	UFBX_ENUM_FORCE_WIDTH(UFBX_CACHE_DATA_FORMAT)
} ufbx_cache_data_format;

UFBX_ENUM_TYPE(ufbx_cache_data_format, UFBX_CACHE_DATA_FORMAT, UFBX_CACHE_DATA_FORMAT_VEC3_DOUBLE);

typedef enum ufbx_cache_data_encoding UFBX_ENUM_REPR {
	UFBX_CACHE_DATA_ENCODING_UNKNOWN,       
	UFBX_CACHE_DATA_ENCODING_LITTLE_ENDIAN, 
	UFBX_CACHE_DATA_ENCODING_BIG_ENDIAN,    

	UFBX_ENUM_FORCE_WIDTH(UFBX_CACHE_DATA_ENCODING)
} ufbx_cache_data_encoding;

UFBX_ENUM_TYPE(ufbx_cache_data_encoding, UFBX_CACHE_DATA_ENCODING, UFBX_CACHE_DATA_ENCODING_BIG_ENDIAN);


typedef enum ufbx_cache_interpretation UFBX_ENUM_REPR {
	
	UFBX_CACHE_INTERPRETATION_UNKNOWN,

	
	
	UFBX_CACHE_INTERPRETATION_POINTS,

	
	UFBX_CACHE_INTERPRETATION_VERTEX_POSITION,

	
	UFBX_CACHE_INTERPRETATION_VERTEX_NORMAL,

	UFBX_ENUM_FORCE_WIDTH(UFBX_CACHE_INTERPRETATION)
} ufbx_cache_interpretation;

UFBX_ENUM_TYPE(ufbx_cache_interpretation, UFBX_CACHE_INTERPRETATION, UFBX_CACHE_INTERPRETATION_VERTEX_NORMAL);

typedef struct ufbx_cache_frame {

	
	ufbx_string channel;

	
	double time;

	
	
	
	ufbx_string filename;

	
	ufbx_cache_file_format file_format;

	
	ufbx_mirror_axis mirror_axis;

	
	ufbx_real scale_factor;

	ufbx_cache_data_format data_format;     
	ufbx_cache_data_encoding data_encoding; 
	uint64_t data_offset;                   
	uint32_t data_count;                    
	uint32_t data_element_bytes;            
	uint64_t data_total_bytes;              
} ufbx_cache_frame;

UFBX_LIST_TYPE(ufbx_cache_frame_list, ufbx_cache_frame);

typedef struct ufbx_cache_channel {

	
	ufbx_string name;

	
	ufbx_cache_interpretation interpretation;

	
	
	ufbx_string interpretation_name;

	
	
	ufbx_cache_frame_list frames;

	
	ufbx_mirror_axis mirror_axis;

	
	ufbx_real scale_factor;

} ufbx_cache_channel;

UFBX_LIST_TYPE(ufbx_cache_channel_list, ufbx_cache_channel);

typedef struct ufbx_geometry_cache {
	ufbx_string root_filename;
	ufbx_cache_channel_list channels;
	ufbx_cache_frame_list frames;
	ufbx_string_list extra_info;
} ufbx_geometry_cache;

struct ufbx_cache_deformer {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	ufbx_string channel;
	ufbx_nullable ufbx_cache_file *file;

	
	ufbx_nullable ufbx_geometry_cache *external_cache;
	ufbx_nullable ufbx_cache_channel *external_channel;
};

struct ufbx_cache_file {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	
	
	ufbx_string filename;
	
	ufbx_string absolute_filename;
	
	
	ufbx_string relative_filename;

	
	
	
	ufbx_blob raw_filename;
	
	ufbx_blob raw_absolute_filename;
	
	
	ufbx_blob raw_relative_filename;

	ufbx_cache_file_format format;

	
	ufbx_nullable ufbx_geometry_cache *external_cache;
};




typedef struct ufbx_material_map {

	
	
	
	union {
		ufbx_real value_real;
		ufbx_vec2 value_vec2;
		ufbx_vec3 value_vec3;
		ufbx_vec4 value_vec4;
	};
	int64_t value_int;

	
	
	ufbx_nullable ufbx_texture *texture;

	
	
	
	bool has_value;

	
	
	bool texture_enabled;

	
	bool feature_disabled;

	
	uint8_t value_components;

} ufbx_material_map;


typedef struct ufbx_material_feature_info {

	
	
	bool enabled;

	
	bool is_explicit;

} ufbx_material_feature_info;


typedef struct ufbx_material_texture {
	ufbx_string material_prop; 
	ufbx_string shader_prop;   

	
	ufbx_texture *texture;

} ufbx_material_texture;

UFBX_LIST_TYPE(ufbx_material_texture_list, ufbx_material_texture);


typedef enum ufbx_shader_type UFBX_ENUM_REPR {
	
	UFBX_SHADER_UNKNOWN,
	
	UFBX_SHADER_FBX_LAMBERT,
	
	UFBX_SHADER_FBX_PHONG,
	
	
	UFBX_SHADER_OSL_STANDARD_SURFACE,
	
	
	UFBX_SHADER_ARNOLD_STANDARD_SURFACE,
	
	
	UFBX_SHADER_3DS_MAX_PHYSICAL_MATERIAL,
	
	
	UFBX_SHADER_3DS_MAX_PBR_METAL_ROUGH,
	
	
	UFBX_SHADER_3DS_MAX_PBR_SPEC_GLOSS,
	
	
	UFBX_SHADER_GLTF_MATERIAL,
	
	
	UFBX_SHADER_OPENPBR_MATERIAL,
	
	
	UFBX_SHADER_SHADERFX_GRAPH,
	
	
	
	UFBX_SHADER_BLENDER_PHONG,
	
	UFBX_SHADER_WAVEFRONT_MTL,

	UFBX_ENUM_FORCE_WIDTH(UFBX_SHADER_TYPE)
} ufbx_shader_type;

UFBX_ENUM_TYPE(ufbx_shader_type, UFBX_SHADER_TYPE, UFBX_SHADER_WAVEFRONT_MTL);


typedef enum ufbx_material_fbx_map UFBX_ENUM_REPR {
	UFBX_MATERIAL_FBX_DIFFUSE_FACTOR,
	UFBX_MATERIAL_FBX_DIFFUSE_COLOR,
	UFBX_MATERIAL_FBX_SPECULAR_FACTOR,
	UFBX_MATERIAL_FBX_SPECULAR_COLOR,
	UFBX_MATERIAL_FBX_SPECULAR_EXPONENT,
	UFBX_MATERIAL_FBX_REFLECTION_FACTOR,
	UFBX_MATERIAL_FBX_REFLECTION_COLOR,
	UFBX_MATERIAL_FBX_TRANSPARENCY_FACTOR,
	UFBX_MATERIAL_FBX_TRANSPARENCY_COLOR,
	UFBX_MATERIAL_FBX_EMISSION_FACTOR,
	UFBX_MATERIAL_FBX_EMISSION_COLOR,
	UFBX_MATERIAL_FBX_AMBIENT_FACTOR,
	UFBX_MATERIAL_FBX_AMBIENT_COLOR,
	UFBX_MATERIAL_FBX_NORMAL_MAP,
	UFBX_MATERIAL_FBX_BUMP,
	UFBX_MATERIAL_FBX_BUMP_FACTOR,
	UFBX_MATERIAL_FBX_DISPLACEMENT_FACTOR,
	UFBX_MATERIAL_FBX_DISPLACEMENT,
	UFBX_MATERIAL_FBX_VECTOR_DISPLACEMENT_FACTOR,
	UFBX_MATERIAL_FBX_VECTOR_DISPLACEMENT,

	UFBX_ENUM_FORCE_WIDTH(UFBX_MATERIAL_FBX_MAP)
} ufbx_material_fbx_map;

UFBX_ENUM_TYPE(ufbx_material_fbx_map, UFBX_MATERIAL_FBX_MAP, UFBX_MATERIAL_FBX_VECTOR_DISPLACEMENT);


typedef enum ufbx_material_pbr_map UFBX_ENUM_REPR {
	UFBX_MATERIAL_PBR_BASE_FACTOR,
	UFBX_MATERIAL_PBR_BASE_COLOR,
	UFBX_MATERIAL_PBR_ROUGHNESS,
	UFBX_MATERIAL_PBR_METALNESS,
	UFBX_MATERIAL_PBR_DIFFUSE_ROUGHNESS,
	UFBX_MATERIAL_PBR_SPECULAR_FACTOR,
	UFBX_MATERIAL_PBR_SPECULAR_COLOR,
	UFBX_MATERIAL_PBR_SPECULAR_IOR,
	UFBX_MATERIAL_PBR_SPECULAR_ANISOTROPY,
	UFBX_MATERIAL_PBR_SPECULAR_ROTATION,
	UFBX_MATERIAL_PBR_TRANSMISSION_FACTOR,
	UFBX_MATERIAL_PBR_TRANSMISSION_COLOR,
	UFBX_MATERIAL_PBR_TRANSMISSION_DEPTH,
	UFBX_MATERIAL_PBR_TRANSMISSION_SCATTER,
	UFBX_MATERIAL_PBR_TRANSMISSION_SCATTER_ANISOTROPY,
	UFBX_MATERIAL_PBR_TRANSMISSION_DISPERSION,
	UFBX_MATERIAL_PBR_TRANSMISSION_ROUGHNESS,
	UFBX_MATERIAL_PBR_TRANSMISSION_EXTRA_ROUGHNESS,
	UFBX_MATERIAL_PBR_TRANSMISSION_PRIORITY,
	UFBX_MATERIAL_PBR_TRANSMISSION_ENABLE_IN_AOV,
	UFBX_MATERIAL_PBR_SUBSURFACE_FACTOR,
	UFBX_MATERIAL_PBR_SUBSURFACE_COLOR,
	UFBX_MATERIAL_PBR_SUBSURFACE_RADIUS,
	UFBX_MATERIAL_PBR_SUBSURFACE_SCALE,
	UFBX_MATERIAL_PBR_SUBSURFACE_ANISOTROPY,
	UFBX_MATERIAL_PBR_SUBSURFACE_TINT_COLOR,
	UFBX_MATERIAL_PBR_SUBSURFACE_TYPE,
	UFBX_MATERIAL_PBR_SHEEN_FACTOR,
	UFBX_MATERIAL_PBR_SHEEN_COLOR,
	UFBX_MATERIAL_PBR_SHEEN_ROUGHNESS,
	UFBX_MATERIAL_PBR_COAT_FACTOR,
	UFBX_MATERIAL_PBR_COAT_COLOR,
	UFBX_MATERIAL_PBR_COAT_ROUGHNESS,
	UFBX_MATERIAL_PBR_COAT_IOR,
	UFBX_MATERIAL_PBR_COAT_ANISOTROPY,
	UFBX_MATERIAL_PBR_COAT_ROTATION,
	UFBX_MATERIAL_PBR_COAT_NORMAL,
	UFBX_MATERIAL_PBR_COAT_AFFECT_BASE_COLOR,
	UFBX_MATERIAL_PBR_COAT_AFFECT_BASE_ROUGHNESS,
	UFBX_MATERIAL_PBR_THIN_FILM_FACTOR,
	UFBX_MATERIAL_PBR_THIN_FILM_THICKNESS,
	UFBX_MATERIAL_PBR_THIN_FILM_IOR,
	UFBX_MATERIAL_PBR_EMISSION_FACTOR,
	UFBX_MATERIAL_PBR_EMISSION_COLOR,
	UFBX_MATERIAL_PBR_OPACITY,
	UFBX_MATERIAL_PBR_INDIRECT_DIFFUSE,
	UFBX_MATERIAL_PBR_INDIRECT_SPECULAR,
	UFBX_MATERIAL_PBR_NORMAL_MAP,
	UFBX_MATERIAL_PBR_TANGENT_MAP,
	UFBX_MATERIAL_PBR_DISPLACEMENT_MAP,
	UFBX_MATERIAL_PBR_MATTE_FACTOR,
	UFBX_MATERIAL_PBR_MATTE_COLOR,
	UFBX_MATERIAL_PBR_AMBIENT_OCCLUSION,
	UFBX_MATERIAL_PBR_GLOSSINESS,
	UFBX_MATERIAL_PBR_COAT_GLOSSINESS,
	UFBX_MATERIAL_PBR_TRANSMISSION_GLOSSINESS,

	UFBX_ENUM_FORCE_WIDTH(UFBX_MATERIAL_PBR_MAP)
} ufbx_material_pbr_map;

UFBX_ENUM_TYPE(ufbx_material_pbr_map, UFBX_MATERIAL_PBR_MAP, UFBX_MATERIAL_PBR_TRANSMISSION_GLOSSINESS);


typedef enum ufbx_material_feature UFBX_ENUM_REPR {
	UFBX_MATERIAL_FEATURE_PBR,
	UFBX_MATERIAL_FEATURE_METALNESS,
	UFBX_MATERIAL_FEATURE_DIFFUSE,
	UFBX_MATERIAL_FEATURE_SPECULAR,
	UFBX_MATERIAL_FEATURE_EMISSION,
	UFBX_MATERIAL_FEATURE_TRANSMISSION,
	UFBX_MATERIAL_FEATURE_COAT,
	UFBX_MATERIAL_FEATURE_SHEEN,
	UFBX_MATERIAL_FEATURE_OPACITY,
	UFBX_MATERIAL_FEATURE_AMBIENT_OCCLUSION,
	UFBX_MATERIAL_FEATURE_MATTE,
	UFBX_MATERIAL_FEATURE_UNLIT,
	UFBX_MATERIAL_FEATURE_IOR,
	UFBX_MATERIAL_FEATURE_DIFFUSE_ROUGHNESS,
	UFBX_MATERIAL_FEATURE_TRANSMISSION_ROUGHNESS,
	UFBX_MATERIAL_FEATURE_THIN_WALLED,
	UFBX_MATERIAL_FEATURE_CAUSTICS,
	UFBX_MATERIAL_FEATURE_EXIT_TO_BACKGROUND,
	UFBX_MATERIAL_FEATURE_INTERNAL_REFLECTIONS,
	UFBX_MATERIAL_FEATURE_DOUBLE_SIDED,
	UFBX_MATERIAL_FEATURE_ROUGHNESS_AS_GLOSSINESS,
	UFBX_MATERIAL_FEATURE_COAT_ROUGHNESS_AS_GLOSSINESS,
	UFBX_MATERIAL_FEATURE_TRANSMISSION_ROUGHNESS_AS_GLOSSINESS,

	UFBX_ENUM_FORCE_WIDTH(UFBX_MATERIAL_FEATURE)
} ufbx_material_feature;

UFBX_ENUM_TYPE(ufbx_material_feature, UFBX_MATERIAL_FEATURE, UFBX_MATERIAL_FEATURE_TRANSMISSION_ROUGHNESS_AS_GLOSSINESS);

typedef struct ufbx_material_fbx_maps {
	union {
		ufbx_material_map maps[UFBX_MATERIAL_FBX_MAP_COUNT];
		struct {
			ufbx_material_map diffuse_factor;
			ufbx_material_map diffuse_color;
			ufbx_material_map specular_factor;
			ufbx_material_map specular_color;
			ufbx_material_map specular_exponent;
			ufbx_material_map reflection_factor;
			ufbx_material_map reflection_color;
			ufbx_material_map transparency_factor;
			ufbx_material_map transparency_color;
			ufbx_material_map emission_factor;
			ufbx_material_map emission_color;
			ufbx_material_map ambient_factor;
			ufbx_material_map ambient_color;
			ufbx_material_map normal_map;
			ufbx_material_map bump;
			ufbx_material_map bump_factor;
			ufbx_material_map displacement_factor;
			ufbx_material_map displacement;
			ufbx_material_map vector_displacement_factor;
			ufbx_material_map vector_displacement;
		};
	};
} ufbx_material_fbx_maps;

typedef struct ufbx_material_pbr_maps {
	union {
		ufbx_material_map maps[UFBX_MATERIAL_PBR_MAP_COUNT];
		struct {
			ufbx_material_map base_factor;
			ufbx_material_map base_color;
			ufbx_material_map roughness;
			ufbx_material_map metalness;
			ufbx_material_map diffuse_roughness;
			ufbx_material_map specular_factor;
			ufbx_material_map specular_color;
			ufbx_material_map specular_ior;
			ufbx_material_map specular_anisotropy;
			ufbx_material_map specular_rotation;
			ufbx_material_map transmission_factor;
			ufbx_material_map transmission_color;
			ufbx_material_map transmission_depth;
			ufbx_material_map transmission_scatter;
			ufbx_material_map transmission_scatter_anisotropy;
			ufbx_material_map transmission_dispersion;
			ufbx_material_map transmission_roughness;
			ufbx_material_map transmission_extra_roughness;
			ufbx_material_map transmission_priority;
			ufbx_material_map transmission_enable_in_aov;
			ufbx_material_map subsurface_factor;
			ufbx_material_map subsurface_color;
			ufbx_material_map subsurface_radius;
			ufbx_material_map subsurface_scale;
			ufbx_material_map subsurface_anisotropy;
			ufbx_material_map subsurface_tint_color;
			ufbx_material_map subsurface_type;
			ufbx_material_map sheen_factor;
			ufbx_material_map sheen_color;
			ufbx_material_map sheen_roughness;
			ufbx_material_map coat_factor;
			ufbx_material_map coat_color;
			ufbx_material_map coat_roughness;
			ufbx_material_map coat_ior;
			ufbx_material_map coat_anisotropy;
			ufbx_material_map coat_rotation;
			ufbx_material_map coat_normal;
			ufbx_material_map coat_affect_base_color;
			ufbx_material_map coat_affect_base_roughness;
			ufbx_material_map thin_film_factor;
			ufbx_material_map thin_film_thickness;
			ufbx_material_map thin_film_ior;
			ufbx_material_map emission_factor;
			ufbx_material_map emission_color;
			ufbx_material_map opacity;
			ufbx_material_map indirect_diffuse;
			ufbx_material_map indirect_specular;
			ufbx_material_map normal_map;
			ufbx_material_map tangent_map;
			ufbx_material_map displacement_map;
			ufbx_material_map matte_factor;
			ufbx_material_map matte_color;
			ufbx_material_map ambient_occlusion;
			ufbx_material_map glossiness;
			ufbx_material_map coat_glossiness;
			ufbx_material_map transmission_glossiness;
		};
	};
} ufbx_material_pbr_maps;

typedef struct ufbx_material_features {
	union {
		ufbx_material_feature_info features[UFBX_MATERIAL_FEATURE_COUNT];
		struct {
			ufbx_material_feature_info pbr;
			ufbx_material_feature_info metalness;
			ufbx_material_feature_info diffuse;
			ufbx_material_feature_info specular;
			ufbx_material_feature_info emission;
			ufbx_material_feature_info transmission;
			ufbx_material_feature_info coat;
			ufbx_material_feature_info sheen;
			ufbx_material_feature_info opacity;
			ufbx_material_feature_info ambient_occlusion;
			ufbx_material_feature_info matte;
			ufbx_material_feature_info unlit;
			ufbx_material_feature_info ior;
			ufbx_material_feature_info diffuse_roughness;
			ufbx_material_feature_info transmission_roughness;
			ufbx_material_feature_info thin_walled;
			ufbx_material_feature_info caustics;
			ufbx_material_feature_info exit_to_background;
			ufbx_material_feature_info internal_reflections;
			ufbx_material_feature_info double_sided;
			ufbx_material_feature_info roughness_as_glossiness;
			ufbx_material_feature_info coat_roughness_as_glossiness;
			ufbx_material_feature_info transmission_roughness_as_glossiness;
		};
	};
} ufbx_material_features;



struct ufbx_material {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	
	ufbx_material_fbx_maps fbx;

	
	
	ufbx_material_pbr_maps pbr;

	
	ufbx_material_features features;

	
	ufbx_shader_type shader_type;      
	ufbx_nullable ufbx_shader *shader; 
	ufbx_string shading_model_name;    

	
	
	
	
	ufbx_string shader_prop_prefix;

	
	
	ufbx_material_texture_list textures; 
};

typedef enum ufbx_texture_type UFBX_ENUM_REPR {

	
	
	
	
	UFBX_TEXTURE_FILE,

	
	UFBX_TEXTURE_LAYERED,

	
	UFBX_TEXTURE_PROCEDURAL,

	
	
	UFBX_TEXTURE_SHADER,

	UFBX_ENUM_FORCE_WIDTH(UFBX_TEXTURE_TYPE)
} ufbx_texture_type;

UFBX_ENUM_TYPE(ufbx_texture_type, UFBX_TEXTURE_TYPE, UFBX_TEXTURE_SHADER);





typedef enum ufbx_blend_mode UFBX_ENUM_REPR {
	UFBX_BLEND_TRANSLUCENT,   
	UFBX_BLEND_ADDITIVE,      
	UFBX_BLEND_MULTIPLY,      
	UFBX_BLEND_MULTIPLY_2X,   
	UFBX_BLEND_OVER,          
	UFBX_BLEND_REPLACE,       
	UFBX_BLEND_DISSOLVE,      
	UFBX_BLEND_DARKEN,        
	UFBX_BLEND_COLOR_BURN,    
	UFBX_BLEND_LINEAR_BURN,   
	UFBX_BLEND_DARKER_COLOR,  
	UFBX_BLEND_LIGHTEN,       
	UFBX_BLEND_SCREEN,        
	UFBX_BLEND_COLOR_DODGE,   
	UFBX_BLEND_LINEAR_DODGE,  
	UFBX_BLEND_LIGHTER_COLOR, 
	UFBX_BLEND_SOFT_LIGHT,    
	UFBX_BLEND_HARD_LIGHT,    
	UFBX_BLEND_VIVID_LIGHT,   
	UFBX_BLEND_LINEAR_LIGHT,  
	UFBX_BLEND_PIN_LIGHT,     
	UFBX_BLEND_HARD_MIX,      
	UFBX_BLEND_DIFFERENCE,    
	UFBX_BLEND_EXCLUSION,     
	UFBX_BLEND_SUBTRACT,      
	UFBX_BLEND_DIVIDE,        
	UFBX_BLEND_HUE,           
	UFBX_BLEND_SATURATION,    
	UFBX_BLEND_COLOR,         
	UFBX_BLEND_LUMINOSITY,    
	UFBX_BLEND_OVERLAY,       

	UFBX_ENUM_FORCE_WIDTH(UFBX_BLEND_MODE)
} ufbx_blend_mode;

UFBX_ENUM_TYPE(ufbx_blend_mode, UFBX_BLEND_MODE, UFBX_BLEND_OVERLAY);


typedef enum ufbx_wrap_mode UFBX_ENUM_REPR {
	UFBX_WRAP_REPEAT, 
	UFBX_WRAP_CLAMP,  

	UFBX_ENUM_FORCE_WIDTH(UFBX_WRAP_MODE)
} ufbx_wrap_mode;

UFBX_ENUM_TYPE(ufbx_wrap_mode, UFBX_WRAP_MODE, UFBX_WRAP_CLAMP);


typedef struct ufbx_texture_layer {
	ufbx_texture *texture;      
	ufbx_blend_mode blend_mode; 
	ufbx_real alpha;            
} ufbx_texture_layer;

UFBX_LIST_TYPE(ufbx_texture_layer_list, ufbx_texture_layer);

typedef enum ufbx_shader_texture_type UFBX_ENUM_REPR {
	UFBX_SHADER_TEXTURE_UNKNOWN,

	
	
	
	UFBX_SHADER_TEXTURE_SELECT_OUTPUT,

	
	
	UFBX_SHADER_TEXTURE_OSL,

	UFBX_ENUM_FORCE_WIDTH(UFBX_SHADER_TEXTURE_TYPE)
} ufbx_shader_texture_type;

UFBX_ENUM_TYPE(ufbx_shader_texture_type, UFBX_SHADER_TEXTURE_TYPE, UFBX_SHADER_TEXTURE_OSL);


typedef struct ufbx_shader_texture_input {

	
	ufbx_string name;

	
	union {
		ufbx_real value_real;
		ufbx_vec2 value_vec2;
		ufbx_vec3 value_vec3;
		ufbx_vec4 value_vec4;
	};
	int64_t value_int;
	ufbx_string value_str;
	ufbx_blob value_blob;

	
	ufbx_nullable ufbx_texture *texture;

	
	int64_t texture_output_index;

	
	
	bool texture_enabled;

	
	ufbx_prop *prop;

	
	ufbx_nullable ufbx_prop *texture_prop;

	
	ufbx_nullable ufbx_prop *texture_enabled_prop;

} ufbx_shader_texture_input;

UFBX_LIST_TYPE(ufbx_shader_texture_input_list, ufbx_shader_texture_input);








typedef struct ufbx_shader_texture {

	
	ufbx_shader_texture_type type;

	
	ufbx_string shader_name;

	
	uint64_t shader_type_id;

	
	
	ufbx_shader_texture_input_list inputs;

	
	ufbx_string shader_source;
	ufbx_blob raw_shader_source;

	
	
	
	ufbx_texture *main_texture;

	
	int64_t main_texture_output_index;

	
	
	ufbx_string prop_prefix;

} ufbx_shader_texture;


typedef struct ufbx_texture_file {

	
	uint32_t index;

	

	
	
	
	ufbx_string filename;
	
	ufbx_string absolute_filename;
	
	
	ufbx_string relative_filename;

	
	
	
	ufbx_blob raw_filename;
	
	ufbx_blob raw_absolute_filename;
	
	
	ufbx_blob raw_relative_filename;

	
	ufbx_blob content;

} ufbx_texture_file;

UFBX_LIST_TYPE(ufbx_texture_file_list, ufbx_texture_file);


struct ufbx_texture {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	ufbx_texture_type type;

	

	
	
	
	ufbx_string filename;
	
	ufbx_string absolute_filename;
	
	
	ufbx_string relative_filename;

	
	
	
	ufbx_blob raw_filename;
	
	ufbx_blob raw_absolute_filename;
	
	
	ufbx_blob raw_relative_filename;

	
	ufbx_blob content;

	
	ufbx_nullable ufbx_video *video;

	
	uint32_t file_index;

	
	bool has_file;

	
	ufbx_texture_layer_list layers;

	
	
	
	ufbx_nullable ufbx_shader_texture *shader;

	
	
	ufbx_texture_list file_textures;

	
	ufbx_string uv_set;

	
	ufbx_wrap_mode wrap_u;
	ufbx_wrap_mode wrap_v;

	
	bool has_uv_transform;       
	ufbx_transform uv_transform; 
	ufbx_matrix texture_to_uv;   
	ufbx_matrix uv_to_texture;   
};


struct ufbx_video {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	

	
	
	
	ufbx_string filename;
	
	ufbx_string absolute_filename;
	
	
	ufbx_string relative_filename;

	
	
	
	ufbx_blob raw_filename;
	
	ufbx_blob raw_absolute_filename;
	
	
	ufbx_blob raw_relative_filename;

	
	ufbx_blob content;
};



struct ufbx_shader {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	ufbx_shader_type type;

	

	
	
	ufbx_shader_binding_list bindings;
};


typedef struct ufbx_shader_prop_binding {
	ufbx_string shader_prop;   
	ufbx_string material_prop; 
} ufbx_shader_prop_binding;

UFBX_LIST_TYPE(ufbx_shader_prop_binding_list, ufbx_shader_prop_binding);


struct ufbx_shader_binding {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	ufbx_shader_prop_binding_list prop_bindings; 
};



typedef struct ufbx_prop_override {
	uint32_t element_id;

	uint32_t _internal_key;

	ufbx_string prop_name;
	ufbx_vec4 value;
	ufbx_string value_str;
	int64_t value_int;
} ufbx_prop_override;

UFBX_LIST_TYPE(ufbx_prop_override_list, ufbx_prop_override);

typedef struct ufbx_transform_override {
	uint32_t node_id;
	ufbx_transform transform;
} ufbx_transform_override;

UFBX_LIST_TYPE(ufbx_transform_override_list, ufbx_transform_override);







typedef struct ufbx_anim {

	
	double time_begin;
	double time_end;

	
	ufbx_anim_layer_list layers;

	
	ufbx_real_list override_layer_weights;

	
	ufbx_prop_override_list prop_overrides;

	
	ufbx_transform_override_list transform_overrides;

	
	bool ignore_connections;

	
	bool custom;

} ufbx_anim;

struct ufbx_anim_stack {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	double time_begin;
	double time_end;

	ufbx_anim_layer_list layers;
	ufbx_anim *anim;
};

typedef struct ufbx_anim_prop {
	ufbx_element *element;

	uint32_t _internal_key;

	ufbx_string prop_name;
	ufbx_anim_value *anim_value;
} ufbx_anim_prop;

UFBX_LIST_TYPE(ufbx_anim_prop_list, ufbx_anim_prop);

struct ufbx_anim_layer {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	ufbx_real weight;
	bool weight_is_animated;
	bool blended;
	bool additive;
	bool compose_rotation;
	bool compose_scale;

	ufbx_anim_value_list anim_values;
	ufbx_anim_prop_list anim_props; 

	ufbx_anim *anim;

	uint32_t _min_element_id;
	uint32_t _max_element_id;
	uint32_t _element_id_bitmask[4];
};

struct ufbx_anim_value {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	ufbx_vec3 default_value;
	ufbx_nullable ufbx_anim_curve *curves[3];
};


typedef enum ufbx_interpolation UFBX_ENUM_REPR {
	UFBX_INTERPOLATION_CONSTANT_PREV, 
	UFBX_INTERPOLATION_CONSTANT_NEXT, 
	UFBX_INTERPOLATION_LINEAR,        
	UFBX_INTERPOLATION_CUBIC,         

	UFBX_ENUM_FORCE_WIDTH(UFBX_INTERPOLATION)
} ufbx_interpolation;

UFBX_ENUM_TYPE(ufbx_interpolation, UFBX_INTERPOLATION, UFBX_INTERPOLATION_CUBIC);

typedef enum ufbx_extrapolation_mode UFBX_ENUM_REPR {
	UFBX_EXTRAPOLATION_CONSTANT,        
	UFBX_EXTRAPOLATION_REPEAT,          
	UFBX_EXTRAPOLATION_MIRROR,          
	UFBX_EXTRAPOLATION_SLOPE,           
	UFBX_EXTRAPOLATION_REPEAT_RELATIVE, 

	UFBX_ENUM_FORCE_WIDTH(UFBX_EXTRAPOLATION)
} ufbx_extrapolation_mode;

UFBX_ENUM_TYPE(ufbx_extrapolation_mode, UFBX_EXTRAPOLATION_MODE, UFBX_EXTRAPOLATION_REPEAT_RELATIVE);

typedef struct ufbx_extrapolation {
	ufbx_extrapolation_mode mode;

	
	
	int32_t repeat_count;
} ufbx_extrapolation;


typedef struct ufbx_tangent {
	float dx; 
	float dy; 
} ufbx_tangent;













typedef struct ufbx_keyframe {
	double time;
	ufbx_real value;
	ufbx_interpolation interpolation;
	ufbx_tangent left;
	ufbx_tangent right;
} ufbx_keyframe;

UFBX_LIST_TYPE(ufbx_keyframe_list, ufbx_keyframe);

struct ufbx_anim_curve {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	ufbx_keyframe_list keyframes;

	
	ufbx_extrapolation pre_extrapolation;
	
	ufbx_extrapolation post_extrapolation;

	
	ufbx_real min_value;
	ufbx_real max_value;

	
	double min_time;
	double max_time;
};




struct ufbx_display_layer {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	ufbx_node_list nodes;

	
	bool visible; 
	bool frozen;  

	ufbx_vec3 ui_color; 
};


struct ufbx_selection_set {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	ufbx_selection_node_list nodes;
};


struct ufbx_selection_node {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	ufbx_nullable ufbx_node *target_node;
	ufbx_nullable ufbx_mesh *target_mesh;
	bool include_node; 

	
	
	
	ufbx_uint32_list vertices; 
	ufbx_uint32_list edges;    
	ufbx_uint32_list faces;    
};



struct ufbx_character {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };
};


typedef enum ufbx_constraint_type UFBX_ENUM_REPR {
	UFBX_CONSTRAINT_UNKNOWN,
	UFBX_CONSTRAINT_AIM,
	UFBX_CONSTRAINT_PARENT,
	UFBX_CONSTRAINT_POSITION,
	UFBX_CONSTRAINT_ROTATION,
	UFBX_CONSTRAINT_SCALE,
	
	
	UFBX_CONSTRAINT_SINGLE_CHAIN_IK,

	UFBX_ENUM_FORCE_WIDTH(UFBX_CONSTRAINT_TYPE)
} ufbx_constraint_type;

UFBX_ENUM_TYPE(ufbx_constraint_type, UFBX_CONSTRAINT_TYPE, UFBX_CONSTRAINT_SINGLE_CHAIN_IK);


typedef struct ufbx_constraint_target {
	ufbx_node *node;          
	ufbx_real weight;         
	ufbx_transform transform; 
} ufbx_constraint_target;

UFBX_LIST_TYPE(ufbx_constraint_target_list, ufbx_constraint_target);


typedef enum ufbx_constraint_aim_up_type UFBX_ENUM_REPR {
	UFBX_CONSTRAINT_AIM_UP_SCENE,      
	UFBX_CONSTRAINT_AIM_UP_TO_NODE,    
	UFBX_CONSTRAINT_AIM_UP_ALIGN_NODE, 
	UFBX_CONSTRAINT_AIM_UP_VECTOR,     
	UFBX_CONSTRAINT_AIM_UP_NONE,       

	UFBX_ENUM_FORCE_WIDTH(UFBX_CONSTRAINT_AIM_UP_TYPE)
} ufbx_constraint_aim_up_type;

UFBX_ENUM_TYPE(ufbx_constraint_aim_up_type, UFBX_CONSTRAINT_AIM_UP_TYPE, UFBX_CONSTRAINT_AIM_UP_NONE);


typedef enum ufbx_constraint_ik_pole_type UFBX_ENUM_REPR {
	UFBX_CONSTRAINT_IK_POLE_VECTOR, 
	UFBX_CONSTRAINT_IK_POLE_NODE,   

	UFBX_ENUM_FORCE_WIDTH(UFBX_CONSTRAINT_IK_POLE_TYPE)
} ufbx_constraint_ik_pole_type;

UFBX_ENUM_TYPE(ufbx_constraint_ik_pole_type, UFBX_CONSTRAINT_IK_POLE_TYPE, UFBX_CONSTRAINT_IK_POLE_NODE);

struct ufbx_constraint {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	ufbx_constraint_type type;
	ufbx_string type_name;

	
	ufbx_nullable ufbx_node *node;

	
	ufbx_constraint_target_list targets;

	
	ufbx_real weight;
	bool active;

	
	bool constrain_translation[3];
	bool constrain_rotation[3];
	bool constrain_scale[3];

	
	ufbx_transform transform_offset;

	
	ufbx_vec3 aim_vector;
	ufbx_constraint_aim_up_type aim_up_type;
	ufbx_nullable ufbx_node *aim_up_node;
	ufbx_vec3 aim_up_vector;

	
	ufbx_nullable ufbx_node *ik_effector;
	ufbx_nullable ufbx_node *ik_end_node;
	ufbx_vec3 ik_pole_vector;
};



struct ufbx_audio_layer {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	ufbx_audio_clip_list clips;
};

struct ufbx_audio_clip {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	
	
	ufbx_string filename;
	
	ufbx_string absolute_filename;
	
	
	ufbx_string relative_filename;

	
	
	
	ufbx_blob raw_filename;
	
	ufbx_blob raw_absolute_filename;
	
	
	ufbx_blob raw_relative_filename;

	
	ufbx_blob content;
};



typedef struct ufbx_bone_pose {

	
	ufbx_node *bone_node;

	
	ufbx_matrix bone_to_world;

	
	
	
	ufbx_matrix bone_to_parent;

} ufbx_bone_pose;

UFBX_LIST_TYPE(ufbx_bone_pose_list, ufbx_bone_pose);

struct ufbx_pose {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };

	
	bool is_bind_pose;

	
	
	ufbx_bone_pose_list bone_poses;
};

struct ufbx_metadata_object {
	union { ufbx_element element; struct {
		ufbx_string name;
		ufbx_props props;
		uint32_t element_id;
		uint32_t typed_id;
	}; };
};



typedef struct ufbx_name_element {
	ufbx_string name;
	ufbx_element_type type;

	uint32_t _internal_key;

	ufbx_element *element;
} ufbx_name_element;

UFBX_LIST_TYPE(ufbx_name_element_list, ufbx_name_element);





typedef enum ufbx_exporter UFBX_ENUM_REPR {
	UFBX_EXPORTER_UNKNOWN,
	UFBX_EXPORTER_FBX_SDK,
	UFBX_EXPORTER_BLENDER_BINARY,
	UFBX_EXPORTER_BLENDER_ASCII,
	UFBX_EXPORTER_MOTION_BUILDER,

	UFBX_ENUM_FORCE_WIDTH(UFBX_EXPORTER)
} ufbx_exporter;

UFBX_ENUM_TYPE(ufbx_exporter, UFBX_EXPORTER, UFBX_EXPORTER_MOTION_BUILDER);

typedef struct ufbx_application {
	ufbx_string vendor;
	ufbx_string name;
	ufbx_string version;
} ufbx_application;

typedef enum ufbx_file_format UFBX_ENUM_REPR {
	UFBX_FILE_FORMAT_UNKNOWN, 
	UFBX_FILE_FORMAT_FBX,     
	UFBX_FILE_FORMAT_OBJ,     
	UFBX_FILE_FORMAT_MTL,     

	UFBX_ENUM_FORCE_WIDTH(UFBX_FILE_FORMAT)
} ufbx_file_format;

UFBX_ENUM_TYPE(ufbx_file_format, UFBX_FILE_FORMAT, UFBX_FILE_FORMAT_MTL);

typedef enum ufbx_warning_type UFBX_ENUM_REPR {
	
	
	UFBX_WARNING_MISSING_EXTERNAL_FILE,

	
	
	UFBX_WARNING_IMPLICIT_MTL,

	
	UFBX_WARNING_TRUNCATED_ARRAY,

	
	UFBX_WARNING_MISSING_GEOMETRY_DATA,

	
	UFBX_WARNING_DUPLICATE_CONNECTION,

	
	UFBX_WARNING_BAD_VERTEX_W_ATTRIBUTE,

	
	UFBX_WARNING_MISSING_POLYGON_MAPPING,

	
	
	UFBX_WARNING_UNSUPPORTED_VERSION,

	
	
	UFBX_WARNING_INDEX_CLAMPED,

	
	
	UFBX_WARNING_BAD_UNICODE,

	
	UFBX_WARNING_BAD_BASE64_CONTENT,

	
	UFBX_WARNING_BAD_ELEMENT_CONNECTED_TO_ROOT,

	
	UFBX_WARNING_DUPLICATE_OBJECT_ID,

	
	
	UFBX_WARNING_EMPTY_FACE_REMOVED,

	
	UFBX_WARNING_UNKNOWN_OBJ_DIRECTIVE,

	
	
	UFBX_WARNING_TYPE_FIRST_DEDUPLICATED = UFBX_WARNING_INDEX_CLAMPED,

	UFBX_ENUM_FORCE_WIDTH(UFBX_WARNING_TYPE)
} ufbx_warning_type;

UFBX_ENUM_TYPE(ufbx_warning_type, UFBX_WARNING_TYPE, UFBX_WARNING_UNKNOWN_OBJ_DIRECTIVE);




typedef struct ufbx_warning {
	
	ufbx_warning_type type;
	
	ufbx_string description;
	
	uint32_t element_id;
	
	size_t count;
} ufbx_warning;

UFBX_LIST_TYPE(ufbx_warning_list, ufbx_warning);

typedef enum ufbx_thumbnail_format UFBX_ENUM_REPR {
	UFBX_THUMBNAIL_FORMAT_UNKNOWN, 
	UFBX_THUMBNAIL_FORMAT_RGB_24,  
	UFBX_THUMBNAIL_FORMAT_RGBA_32, 

	UFBX_ENUM_FORCE_WIDTH(UFBX_THUMBNAIL_FORMAT)
} ufbx_thumbnail_format;

UFBX_ENUM_TYPE(ufbx_thumbnail_format, UFBX_THUMBNAIL_FORMAT, UFBX_THUMBNAIL_FORMAT_RGBA_32);




typedef enum ufbx_space_conversion UFBX_ENUM_REPR {

	
	
	UFBX_SPACE_CONVERSION_TRANSFORM_ROOT,

	
	
	
	
	UFBX_SPACE_CONVERSION_ADJUST_TRANSFORMS,

	
	
	
	UFBX_SPACE_CONVERSION_MODIFY_GEOMETRY,

	UFBX_ENUM_FORCE_WIDTH(UFBX_SPACE_CONVERSION)
} ufbx_space_conversion;

UFBX_ENUM_TYPE(ufbx_space_conversion, UFBX_SPACE_CONVERSION, UFBX_SPACE_CONVERSION_MODIFY_GEOMETRY);







typedef enum ufbx_geometry_transform_handling UFBX_ENUM_REPR {

	
	
	
	
	UFBX_GEOMETRY_TRANSFORM_HANDLING_PRESERVE,

	
	
	
	UFBX_GEOMETRY_TRANSFORM_HANDLING_HELPER_NODES,

	
	
	
	
	UFBX_GEOMETRY_TRANSFORM_HANDLING_MODIFY_GEOMETRY,

	
	
	UFBX_GEOMETRY_TRANSFORM_HANDLING_MODIFY_GEOMETRY_NO_FALLBACK,

	UFBX_ENUM_FORCE_WIDTH(UFBX_GEOMETRY_TRANSFORM_HANDLING)
} ufbx_geometry_transform_handling;

UFBX_ENUM_TYPE(ufbx_geometry_transform_handling, UFBX_GEOMETRY_TRANSFORM_HANDLING, UFBX_GEOMETRY_TRANSFORM_HANDLING_MODIFY_GEOMETRY_NO_FALLBACK);


typedef enum ufbx_inherit_mode_handling UFBX_ENUM_REPR {

	
	
	
	UFBX_INHERIT_MODE_HANDLING_PRESERVE,

	
	
	
	UFBX_INHERIT_MODE_HANDLING_HELPER_NODES,

	
	
	
	
	UFBX_INHERIT_MODE_HANDLING_COMPENSATE,

	
	
	UFBX_INHERIT_MODE_HANDLING_COMPENSATE_NO_FALLBACK,

	
	
	
	
	UFBX_INHERIT_MODE_HANDLING_IGNORE,

	UFBX_ENUM_FORCE_WIDTH(UFBX_INHERIT_MODE_HANDLING)
} ufbx_inherit_mode_handling;

UFBX_ENUM_TYPE(ufbx_inherit_mode_handling, UFBX_INHERIT_MODE_HANDLING, UFBX_INHERIT_MODE_HANDLING_IGNORE);


typedef enum ufbx_pivot_handling UFBX_ENUM_REPR {

	
	UFBX_PIVOT_HANDLING_RETAIN,

	
	
	
	
	UFBX_PIVOT_HANDLING_ADJUST_TO_PIVOT,

	
	
	
	
	
	UFBX_PIVOT_HANDLING_ADJUST_TO_ROTATION_PIVOT,

	UFBX_ENUM_FORCE_WIDTH(UFBX_PIVOT_HANDLING)
} ufbx_pivot_handling;

UFBX_ENUM_TYPE(ufbx_pivot_handling, UFBX_PIVOT_HANDLING, UFBX_PIVOT_HANDLING_ADJUST_TO_ROTATION_PIVOT);


typedef struct ufbx_thumbnail {
	ufbx_props props;

	
	uint32_t width;
	uint32_t height;

	
	ufbx_thumbnail_format format;

	
	
	ufbx_blob data;
} ufbx_thumbnail;


typedef struct ufbx_metadata {

	
	
	
	ufbx_warning_list warnings;

	
	bool ascii;

	
	uint32_t version;

	
	ufbx_file_format file_format;

	
	
	bool may_contain_no_index;

	
	
	bool may_contain_missing_vertex_position;

	
	
	bool may_contain_broken_elements;

	
	
	bool is_unsafe;

	
	
	bool has_warning[UFBX_WARNING_TYPE_COUNT];

	ufbx_string creator;
	bool big_endian;

	ufbx_string filename;
	ufbx_string relative_root;

	ufbx_blob raw_filename;
	ufbx_blob raw_relative_root;

	ufbx_exporter exporter;
	uint32_t exporter_version;

	ufbx_props scene_props;

	ufbx_application original_application;
	ufbx_application latest_application;

	ufbx_thumbnail thumbnail;

	bool geometry_ignored;
	bool animation_ignored;
	bool embedded_ignored;

	size_t max_face_triangles;

	size_t result_memory_used;
	size_t temp_memory_used;
	size_t result_allocs;
	size_t temp_allocs;

	size_t element_buffer_size;
	size_t num_shader_textures;

	ufbx_real bone_prop_size_unit;
	bool bone_prop_limb_length_relative;

	ufbx_real ortho_size_unit;

	int64_t ktime_second; 

	ufbx_string original_file_path;
	ufbx_blob raw_original_file_path;

	
	ufbx_space_conversion space_conversion;
	ufbx_geometry_transform_handling geometry_transform_handling;
	ufbx_inherit_mode_handling inherit_mode_handling;
	ufbx_pivot_handling pivot_handling;
	ufbx_mirror_axis handedness_conversion_axis;

	
	ufbx_quat root_rotation;
	ufbx_real root_scale;

	
	
	ufbx_mirror_axis mirror_axis;

	
	
	ufbx_real geometry_scale;

} ufbx_metadata;

typedef enum ufbx_time_mode UFBX_ENUM_REPR {
	UFBX_TIME_MODE_DEFAULT,
	UFBX_TIME_MODE_120_FPS,
	UFBX_TIME_MODE_100_FPS,
	UFBX_TIME_MODE_60_FPS,
	UFBX_TIME_MODE_50_FPS,
	UFBX_TIME_MODE_48_FPS,
	UFBX_TIME_MODE_30_FPS,
	UFBX_TIME_MODE_30_FPS_DROP,
	UFBX_TIME_MODE_NTSC_DROP_FRAME,
	UFBX_TIME_MODE_NTSC_FULL_FRAME,
	UFBX_TIME_MODE_PAL,
	UFBX_TIME_MODE_24_FPS,
	UFBX_TIME_MODE_1000_FPS,
	UFBX_TIME_MODE_FILM_FULL_FRAME,
	UFBX_TIME_MODE_CUSTOM,
	UFBX_TIME_MODE_96_FPS,
	UFBX_TIME_MODE_72_FPS,
	UFBX_TIME_MODE_59_94_FPS,

	UFBX_ENUM_FORCE_WIDTH(UFBX_TIME_MODE)
} ufbx_time_mode;

UFBX_ENUM_TYPE(ufbx_time_mode, UFBX_TIME_MODE, UFBX_TIME_MODE_59_94_FPS);

typedef enum ufbx_time_protocol UFBX_ENUM_REPR {
	UFBX_TIME_PROTOCOL_SMPTE,
	UFBX_TIME_PROTOCOL_FRAME_COUNT,
	UFBX_TIME_PROTOCOL_DEFAULT,

	UFBX_ENUM_FORCE_WIDTH(UFBX_TIME_PROTOCOL)
} ufbx_time_protocol;

UFBX_ENUM_TYPE(ufbx_time_protocol, UFBX_TIME_PROTOCOL, UFBX_TIME_PROTOCOL_DEFAULT);

typedef enum ufbx_snap_mode UFBX_ENUM_REPR {
	UFBX_SNAP_MODE_NONE,
	UFBX_SNAP_MODE_SNAP,
	UFBX_SNAP_MODE_PLAY,
	UFBX_SNAP_MODE_SNAP_AND_PLAY,

	UFBX_ENUM_FORCE_WIDTH(UFBX_SNAP_MODE)
} ufbx_snap_mode;

UFBX_ENUM_TYPE(ufbx_snap_mode, UFBX_SNAP_MODE, UFBX_SNAP_MODE_SNAP_AND_PLAY);


typedef struct ufbx_scene_settings {
	ufbx_props props;

	
	
	
	ufbx_coordinate_axes axes;

	
	
	
	ufbx_real unit_meters;

	
	double frames_per_second;

	ufbx_vec3 ambient_color;
	ufbx_string default_camera;

	
	
	ufbx_time_mode time_mode;
	ufbx_time_protocol time_protocol;
	ufbx_snap_mode snap_mode;

	
	ufbx_coordinate_axis original_axis_up;
	ufbx_real original_unit_meters;
} ufbx_scene_settings;

struct ufbx_scene {
	ufbx_metadata metadata;

	
	ufbx_scene_settings settings;

	
	ufbx_node *root_node;

	
	ufbx_anim *anim;

	union {
		struct {
			ufbx_unknown_list unknowns;

			
			ufbx_node_list nodes;

			
			ufbx_mesh_list meshes;
			ufbx_light_list lights;
			ufbx_camera_list cameras;
			ufbx_bone_list bones;
			ufbx_empty_list empties;

			
			ufbx_line_curve_list line_curves;
			ufbx_nurbs_curve_list nurbs_curves;
			ufbx_nurbs_surface_list nurbs_surfaces;
			ufbx_nurbs_trim_surface_list nurbs_trim_surfaces;
			ufbx_nurbs_trim_boundary_list nurbs_trim_boundaries;

			
			ufbx_procedural_geometry_list procedural_geometries;
			ufbx_stereo_camera_list stereo_cameras;
			ufbx_camera_switcher_list camera_switchers;
			ufbx_marker_list markers;
			ufbx_lod_group_list lod_groups;

			
			ufbx_skin_deformer_list skin_deformers;
			ufbx_skin_cluster_list skin_clusters;
			ufbx_blend_deformer_list blend_deformers;
			ufbx_blend_channel_list blend_channels;
			ufbx_blend_shape_list blend_shapes;
			ufbx_cache_deformer_list cache_deformers;
			ufbx_cache_file_list cache_files;

			
			ufbx_material_list materials;
			ufbx_texture_list textures;
			ufbx_video_list videos;
			ufbx_shader_list shaders;
			ufbx_shader_binding_list shader_bindings;

			
			ufbx_anim_stack_list anim_stacks;
			ufbx_anim_layer_list anim_layers;
			ufbx_anim_value_list anim_values;
			ufbx_anim_curve_list anim_curves;

			
			ufbx_display_layer_list display_layers;
			ufbx_selection_set_list selection_sets;
			ufbx_selection_node_list selection_nodes;

			
			ufbx_character_list characters;
			ufbx_constraint_list constraints;

			
			ufbx_audio_layer_list audio_layers;
			ufbx_audio_clip_list audio_clips;

			
			ufbx_pose_list poses;
			ufbx_metadata_object_list metadata_objects;
		};

		ufbx_element_list elements_by_type[UFBX_ELEMENT_TYPE_COUNT];
	};

	
	ufbx_texture_file_list texture_files;

	
	ufbx_element_list elements;           
	ufbx_connection_list connections_src; 
	ufbx_connection_list connections_dst; 

	
	ufbx_name_element_list elements_by_name;

	
	ufbx_nullable ufbx_dom_node *dom_root;
};



typedef struct ufbx_curve_point {
	bool valid;
	ufbx_vec3 position;
	ufbx_vec3 derivative;
} ufbx_curve_point;

typedef struct ufbx_surface_point {
	bool valid;
	ufbx_vec3 position;
	ufbx_vec3 derivative_u;
	ufbx_vec3 derivative_v;
} ufbx_surface_point;



typedef enum ufbx_topo_flags UFBX_FLAG_REPR {
	UFBX_TOPO_NON_MANIFOLD = 0x1, 

	UFBX_FLAG_FORCE_WIDTH(UFBX_TOPO_FLAGS)
} ufbx_topo_flags;

typedef struct ufbx_topo_edge {
	uint32_t index; 
	uint32_t next;  
	uint32_t prev;  
	uint32_t twin;  
	uint32_t face;  
	uint32_t edge;  

	ufbx_topo_flags flags;
} ufbx_topo_edge;




typedef struct ufbx_vertex_stream {
	void *data;          
	size_t vertex_count; 
	size_t vertex_size;  
} ufbx_vertex_stream;







typedef void *ufbx_alloc_fn(void *user, size_t size);





typedef void *ufbx_realloc_fn(void *user, void *old_ptr, size_t old_size, size_t new_size);


typedef void ufbx_free_fn(void *user, void *ptr, size_t size);


typedef void ufbx_free_allocator_fn(void *user);





typedef struct ufbx_allocator {
	
	ufbx_alloc_fn *alloc_fn;
	ufbx_realloc_fn *realloc_fn;
	ufbx_free_fn *free_fn;
	ufbx_free_allocator_fn *free_allocator_fn;
	void *user;
} ufbx_allocator;

typedef struct ufbx_allocator_opts {
	
	ufbx_allocator allocator;

	
	size_t memory_limit;

	
	size_t allocation_limit;

	
	
	
	
	size_t huge_threshold;

	
	
	
	
	
	
	
	
	
	
	size_t max_chunk_size;

} ufbx_allocator_opts;





typedef size_t ufbx_read_fn(void *user, void *data, size_t size);


typedef bool ufbx_skip_fn(void *user, size_t size);



typedef uint64_t ufbx_size_fn(void *user);


typedef void ufbx_close_fn(void *user);

typedef struct ufbx_stream {
	ufbx_read_fn *read_fn;   
	ufbx_skip_fn *skip_fn;   
	ufbx_size_fn *size_fn;   
	ufbx_close_fn *close_fn; 

	
	void *user;
} ufbx_stream;

typedef enum ufbx_open_file_type UFBX_ENUM_REPR {
	UFBX_OPEN_FILE_MAIN_MODEL,     
	UFBX_OPEN_FILE_GEOMETRY_CACHE, 
	UFBX_OPEN_FILE_OBJ_MTL,        

	UFBX_ENUM_FORCE_WIDTH(UFBX_OPEN_FILE_TYPE)
} ufbx_open_file_type;

UFBX_ENUM_TYPE(ufbx_open_file_type, UFBX_OPEN_FILE_TYPE, UFBX_OPEN_FILE_OBJ_MTL);

typedef uintptr_t ufbx_open_file_context;

typedef struct ufbx_open_file_info {
	
	
	
	ufbx_open_file_context context;

	
	ufbx_open_file_type type;

	
	
	ufbx_blob original_filename;
} ufbx_open_file_info;


typedef bool ufbx_open_file_fn(void *user, ufbx_stream *stream, const char *path, size_t path_len, const ufbx_open_file_info *info);

typedef struct ufbx_open_file_cb {
	ufbx_open_file_fn *fn;
	void *user;

	UFBX_CALLBACK_IMPL(ufbx_open_file_cb, ufbx_open_file_fn, bool,
		(void *user, ufbx_stream *stream, const char *path, size_t path_len, const ufbx_open_file_info *info),
		(stream, path, path_len, info))
} ufbx_open_file_cb;


typedef struct ufbx_open_file_opts {
	uint32_t _begin_zero;

	
	ufbx_allocator_opts allocator;

	
	ufbx_unsafe bool filename_null_terminated;

	uint32_t _end_zero;
} ufbx_open_file_opts;


typedef void ufbx_close_memory_fn(void *user, void *data, size_t data_size);

typedef struct ufbx_close_memory_cb {
	ufbx_close_memory_fn *fn;
	void *user;

	UFBX_CALLBACK_IMPL(ufbx_close_memory_cb, ufbx_close_memory_fn, void,
		(void *user, void *data, size_t data_size),
		(data, data_size))
} ufbx_close_memory_cb;


typedef struct ufbx_open_memory_opts {
	uint32_t _begin_zero;

	
	
	ufbx_allocator_opts allocator;

	
	
	
	
	ufbx_unsafe bool no_copy;

	
	ufbx_close_memory_cb close_cb;

	uint32_t _end_zero;
} ufbx_open_memory_opts;



typedef struct ufbx_error_frame {
	uint32_t source_line;
	ufbx_string function;
	ufbx_string description;
} ufbx_error_frame;


typedef enum ufbx_error_type UFBX_ENUM_REPR {

	
	UFBX_ERROR_NONE,

	
	
	UFBX_ERROR_UNKNOWN,

	
	UFBX_ERROR_FILE_NOT_FOUND,

	
	UFBX_ERROR_EMPTY_FILE,

	
	
	UFBX_ERROR_EXTERNAL_FILE_NOT_FOUND,

	
	UFBX_ERROR_OUT_OF_MEMORY,

	
	UFBX_ERROR_MEMORY_LIMIT,

	
	UFBX_ERROR_ALLOCATION_LIMIT,

	
	UFBX_ERROR_TRUNCATED_FILE,

	
	
	UFBX_ERROR_IO,

	
	UFBX_ERROR_CANCELLED,

	
	
	
	UFBX_ERROR_UNRECOGNIZED_FILE_FORMAT,

	
	
	
	
	UFBX_ERROR_UNINITIALIZED_OPTIONS,

	
	UFBX_ERROR_ZERO_VERTEX_SIZE,

	
	UFBX_ERROR_TRUNCATED_VERTEX_STREAM,

	
	UFBX_ERROR_INVALID_UTF8,

	
	UFBX_ERROR_FEATURE_DISABLED,

	
	
	UFBX_ERROR_BAD_NURBS,

	
	UFBX_ERROR_BAD_INDEX,

	
	UFBX_ERROR_NODE_DEPTH_LIMIT,

	
	
	
	UFBX_ERROR_THREADED_ASCII_PARSE,

	
	UFBX_ERROR_UNSAFE_OPTIONS,

	
	UFBX_ERROR_DUPLICATE_OVERRIDE,

	
	
	UFBX_ERROR_UNSUPPORTED_VERSION,

	UFBX_ENUM_FORCE_WIDTH(UFBX_ERROR_TYPE)
} ufbx_error_type;

UFBX_ENUM_TYPE(ufbx_error_type, UFBX_ERROR_TYPE, UFBX_ERROR_UNSUPPORTED_VERSION);



typedef struct ufbx_error {

	
	ufbx_error_type type;

	
	ufbx_string description;

	
	
	uint32_t stack_size;
	ufbx_error_frame stack[UFBX_ERROR_STACK_MAX_DEPTH];

	
	
	size_t info_length;
	char info[UFBX_ERROR_INFO_LENGTH];

} ufbx_error;




typedef struct ufbx_progress {
	uint64_t bytes_read;
	uint64_t bytes_total;
} ufbx_progress;



typedef enum ufbx_progress_result UFBX_ENUM_REPR {

	
	UFBX_PROGRESS_CONTINUE = 0x100,

	
	UFBX_PROGRESS_CANCEL = 0x200,

	UFBX_ENUM_FORCE_WIDTH(UFBX_PROGRESS_RESULT)
} ufbx_progress_result;



typedef ufbx_progress_result ufbx_progress_fn(void *user, const ufbx_progress *progress);

typedef struct ufbx_progress_cb {
	ufbx_progress_fn *fn;
	void *user;

	UFBX_CALLBACK_IMPL(ufbx_progress_cb, ufbx_progress_fn, ufbx_progress_result,
		(void *user, const ufbx_progress *progress),
		(progress))
} ufbx_progress_cb;



typedef struct ufbx_inflate_input ufbx_inflate_input;
typedef struct ufbx_inflate_retain ufbx_inflate_retain;


struct ufbx_inflate_input {
	
	size_t total_size;

	
	const void *data;
	size_t data_size;

	
	void *buffer;
	size_t buffer_size;

	
	ufbx_read_fn *read_fn;
	void *read_user;

	
	ufbx_progress_cb progress_cb;
	uint64_t progress_interval_hint; 

	
	uint64_t progress_size_before;
	uint64_t progress_size_after;

	
	bool no_header;

	
	bool no_checksum;

	
	size_t internal_fast_bits;
};



struct ufbx_inflate_retain {
	bool initialized;
	uint64_t data[1024];
};

typedef enum ufbx_index_error_handling UFBX_ENUM_REPR {
	
	UFBX_INDEX_ERROR_HANDLING_CLAMP,
	
	
	
	
	UFBX_INDEX_ERROR_HANDLING_NO_INDEX,
	
	UFBX_INDEX_ERROR_HANDLING_ABORT_LOADING,
	
	
	
	
	UFBX_INDEX_ERROR_HANDLING_UNSAFE_IGNORE,

	UFBX_ENUM_FORCE_WIDTH(UFBX_INDEX_ERROR_HANDLING)
} ufbx_index_error_handling;

UFBX_ENUM_TYPE(ufbx_index_error_handling, UFBX_INDEX_ERROR_HANDLING, UFBX_INDEX_ERROR_HANDLING_UNSAFE_IGNORE);

typedef enum ufbx_unicode_error_handling UFBX_ENUM_REPR {
	
	UFBX_UNICODE_ERROR_HANDLING_REPLACEMENT_CHARACTER,
	
	UFBX_UNICODE_ERROR_HANDLING_UNDERSCORE,
	
	UFBX_UNICODE_ERROR_HANDLING_QUESTION_MARK,
	
	UFBX_UNICODE_ERROR_HANDLING_REMOVE,
	
	UFBX_UNICODE_ERROR_HANDLING_ABORT_LOADING,
	
	
	
	UFBX_UNICODE_ERROR_HANDLING_UNSAFE_IGNORE,

	UFBX_ENUM_FORCE_WIDTH(UFBX_UNICODE_ERROR_HANDLING)
} ufbx_unicode_error_handling;

UFBX_ENUM_TYPE(ufbx_unicode_error_handling, UFBX_UNICODE_ERROR_HANDLING, UFBX_UNICODE_ERROR_HANDLING_UNSAFE_IGNORE);

typedef enum ufbx_baked_key_flags UFBX_FLAG_REPR {
	
	UFBX_BAKED_KEY_STEP_LEFT = 0x1,
	
	UFBX_BAKED_KEY_STEP_RIGHT = 0x2,
	
	
	UFBX_BAKED_KEY_STEP_KEY = 0x4,
	
	UFBX_BAKED_KEY_KEYFRAME = 0x8,
	
	
	UFBX_BAKED_KEY_REDUCED = 0x10,

	UFBX_FLAG_FORCE_WIDTH(UFBX_BAKED_KEY)
} ufbx_baked_key_flags;

typedef struct ufbx_baked_vec3 {
	double time;                
	ufbx_vec3 value;            
	ufbx_baked_key_flags flags; 
} ufbx_baked_vec3;

UFBX_LIST_TYPE(ufbx_baked_vec3_list, ufbx_baked_vec3);

typedef struct ufbx_baked_quat {
	double time;                
	ufbx_quat value;            
	ufbx_baked_key_flags flags; 
} ufbx_baked_quat;

UFBX_LIST_TYPE(ufbx_baked_quat_list, ufbx_baked_quat);


typedef struct ufbx_baked_node {

	
	uint32_t typed_id;
	
	uint32_t element_id;

	
	bool constant_translation;
	
	bool constant_rotation;
	
	bool constant_scale;

	
	ufbx_baked_vec3_list translation_keys;
	
	ufbx_baked_quat_list rotation_keys;
	
	ufbx_baked_vec3_list scale_keys;

} ufbx_baked_node;

UFBX_LIST_TYPE(ufbx_baked_node_list, ufbx_baked_node);


typedef struct ufbx_baked_prop {
	
	ufbx_string name;
	
	bool constant_value;
	
	ufbx_baked_vec3_list keys;
} ufbx_baked_prop;

UFBX_LIST_TYPE(ufbx_baked_prop_list, ufbx_baked_prop);


typedef struct ufbx_baked_element {
	
	uint32_t element_id;
	
	ufbx_baked_prop_list props;
} ufbx_baked_element;

UFBX_LIST_TYPE(ufbx_baked_element_list, ufbx_baked_element);

typedef struct ufbx_baked_anim_metadata {
	
	size_t result_memory_used;
	size_t temp_memory_used;
	size_t result_allocs;
	size_t temp_allocs;
} ufbx_baked_anim_metadata;



typedef struct ufbx_baked_anim {

	
	
	
	
	ufbx_baked_node_list nodes;

	
	ufbx_baked_element_list elements;

	
	double playback_time_begin;
	double playback_time_end;
	double playback_duration;

	
	double key_time_min;
	double key_time_max;

	
	ufbx_baked_anim_metadata metadata;

} ufbx_baked_anim;






typedef uintptr_t ufbx_thread_pool_context;


typedef struct ufbx_thread_pool_info {
	uint32_t max_concurrent_tasks;
} ufbx_thread_pool_info;



typedef bool ufbx_thread_pool_init_fn(void *user, ufbx_thread_pool_context ctx, const ufbx_thread_pool_info *info);





typedef void ufbx_thread_pool_run_fn(void *user, ufbx_thread_pool_context ctx, uint32_t group, uint32_t start_index, uint32_t count);



typedef void ufbx_thread_pool_wait_fn(void *user, ufbx_thread_pool_context ctx, uint32_t group, uint32_t max_index);


typedef void ufbx_thread_pool_free_fn(void *user, ufbx_thread_pool_context ctx);













typedef struct ufbx_thread_pool {
	ufbx_thread_pool_init_fn *init_fn; 
	ufbx_thread_pool_run_fn *run_fn;   
	ufbx_thread_pool_wait_fn *wait_fn; 
	ufbx_thread_pool_free_fn *free_fn; 
	void *user;
} ufbx_thread_pool;


typedef struct ufbx_thread_opts {

	
	
	ufbx_thread_pool pool;

	
	
	size_t num_tasks;

	
	
	
	
	size_t memory_limit;

} ufbx_thread_opts;


typedef enum ufbx_evaluate_flags UFBX_FLAG_REPR {

	
	UFBX_EVALUATE_FLAG_NO_EXTRAPOLATION = 0x1,

	UFBX_FLAG_FORCE_WIDTH(ufbx_evaluate_flags)
} ufbx_evaluate_flags;





typedef struct ufbx_load_opts {
	uint32_t _begin_zero;

	ufbx_allocator_opts temp_allocator;   
	ufbx_allocator_opts result_allocator; 
	ufbx_thread_opts thread_opts;         

	
	bool ignore_geometry;    
	bool ignore_animation;   
	bool ignore_embedded;    
	bool ignore_all_content; 

	bool evaluate_skinning; 
	bool evaluate_caches;   

	
	
	
	
	
	
	
	
	
	
	bool load_external_files;

	
	bool ignore_missing_external_files;

	
	
	bool skip_skin_vertices;

	
	bool skip_mesh_parts;

	
	bool clean_skin_weights;

	
	
	
	
	bool use_blender_pbr_material;

	
	bool disable_quirks;

	
	bool strict;

	
	
	
	bool force_single_thread_ascii_parsing;

	
	
	ufbx_unsafe bool allow_unsafe;

	
	ufbx_index_error_handling index_error_handling;

	
	
	
	bool connect_broken_elements;

	
	
	bool allow_nodes_out_of_root;

	
	
	bool allow_missing_vertex_position;

	
	bool allow_empty_faces;

	
	
	bool generate_missing_normals;

	
	bool open_main_file_with_default;

	
	char path_separator;

	
	
	
	
	uint32_t node_depth_limit;

	
	uint64_t file_size_estimate;

	
	size_t read_buffer_size;

	
	
	
	ufbx_string filename;

	
	
	ufbx_blob raw_filename;

	
	ufbx_progress_cb progress_cb;
	uint64_t progress_interval_hint; 

	
	ufbx_open_file_cb open_file_cb;

	
	
	ufbx_geometry_transform_handling geometry_transform_handling;

	
	
	ufbx_inherit_mode_handling inherit_mode_handling;

	
	
	ufbx_space_conversion space_conversion;

	
	
	ufbx_pivot_handling pivot_handling;

	
	bool pivot_handling_retain_empties;

	
	ufbx_mirror_axis handedness_conversion_axis;

	
	bool handedness_conversion_retain_winding;

	
	
	
	bool reverse_winding;

	
	
	ufbx_coordinate_axes target_axes;

	
	
	ufbx_real target_unit_meters;

	
	
	
	ufbx_coordinate_axes target_camera_axes;

	
	
	
	ufbx_coordinate_axes target_light_axes;

	
	
	ufbx_string geometry_transform_helper_name;

	
	
	ufbx_string scale_helper_name;

	
	bool normalize_normals;

	
	bool normalize_tangents;

	
	bool use_root_transform;
	ufbx_transform root_transform;

	
	double key_clamp_threshold;

	
	ufbx_unicode_error_handling unicode_error_handling;

	
	
	bool retain_vertex_attrib_w;

	
	bool retain_dom;

	
	ufbx_file_format file_format;

	
	
	size_t file_format_lookahead;

	
	bool no_format_from_content;

	
	
	
	bool no_format_from_extension;

	
	
	
	
	bool obj_search_mtl_by_filename;

	
	bool obj_merge_objects;

	
	bool obj_merge_groups;

	
	bool obj_split_groups;

	
	
	
	
	ufbx_string obj_mtl_path;

	
	ufbx_blob obj_mtl_data;

	
	
	
	ufbx_real obj_unit_meters;

	
	
	
	ufbx_coordinate_axes obj_axes;

	uint32_t _end_zero;
} ufbx_load_opts;



typedef struct ufbx_evaluate_opts {
	uint32_t _begin_zero;

	ufbx_allocator_opts temp_allocator;   
	ufbx_allocator_opts result_allocator; 

	bool evaluate_skinning; 
	bool evaluate_caches;   

	
	
	uint32_t evaluate_flags;

	
	bool load_external_files;

	
	ufbx_open_file_cb open_file_cb;

	uint32_t _end_zero;
} ufbx_evaluate_opts;

UFBX_LIST_TYPE(ufbx_const_uint32_list, const uint32_t);
UFBX_LIST_TYPE(ufbx_const_real_list, const ufbx_real);

typedef struct ufbx_prop_override_desc {
	
	uint32_t element_id;

	
	ufbx_string prop_name;

	
	
	ufbx_vec4 value;
	ufbx_string value_str;
	int64_t value_int;
} ufbx_prop_override_desc;

UFBX_LIST_TYPE(ufbx_const_prop_override_desc_list, const ufbx_prop_override_desc);

UFBX_LIST_TYPE(ufbx_const_transform_override_list, const ufbx_transform_override);

typedef struct ufbx_anim_opts {
	uint32_t _begin_zero;

	
	
	ufbx_const_uint32_list layer_ids;

	
	ufbx_const_real_list override_layer_weights;

	
	
	ufbx_const_prop_override_desc_list prop_overrides;

	
	
	ufbx_const_transform_override_list transform_overrides;

	
	bool ignore_connections;

	ufbx_allocator_opts result_allocator; 

	uint32_t _end_zero;
} ufbx_anim_opts;


typedef enum ufbx_bake_step_handling UFBX_ENUM_REPR {

	
	UFBX_BAKE_STEP_HANDLING_DEFAULT,

	
	
	UFBX_BAKE_STEP_HANDLING_CUSTOM_DURATION,

	
	
	
	UFBX_BAKE_STEP_HANDLING_IDENTICAL_TIME,

	
	
	
	
	
	UFBX_BAKE_STEP_HANDLING_ADJACENT_DOUBLE,

	
	UFBX_BAKE_STEP_HANDLING_IGNORE,

	UFBX_ENUM_FORCE_WIDTH(ufbx_bake_step_handling)
} ufbx_bake_step_handling;

UFBX_ENUM_TYPE(ufbx_bake_step_handling, UFBX_BAKE_STEP_HANDLING, UFBX_BAKE_STEP_HANDLING_IGNORE);

typedef struct ufbx_bake_opts {
	uint32_t _begin_zero;

	ufbx_allocator_opts temp_allocator;   
	ufbx_allocator_opts result_allocator; 

	
	
	
	
	
	bool trim_start_time;

	
	
	double resample_rate;

	
	
	
	
	double minimum_sample_rate;

	
	
	double maximum_sample_rate;

	
	bool bake_transform_props;

	
	bool skip_node_transforms;

	
	
	bool no_resample_rotation;

	
	bool ignore_layer_weight_animation;

	
	
	size_t max_keyframe_segments;

	
	ufbx_bake_step_handling step_handling;

	
	double step_custom_duration;

	
	
	
	double step_custom_epsilon;

	
	
	uint32_t evaluate_flags;

	
	bool key_reduction_enabled;

	
	
	bool key_reduction_rotation;

	
	
	double key_reduction_threshold;

	
	
	
	size_t key_reduction_passes;

	uint32_t _end_zero;
} ufbx_bake_opts;



typedef struct ufbx_tessellate_curve_opts {
	uint32_t _begin_zero;

	ufbx_allocator_opts temp_allocator;   
	ufbx_allocator_opts result_allocator; 

	
	size_t span_subdivision;

	uint32_t _end_zero;
} ufbx_tessellate_curve_opts;



typedef struct ufbx_tessellate_surface_opts {
	uint32_t _begin_zero;

	ufbx_allocator_opts temp_allocator;   
	ufbx_allocator_opts result_allocator; 

	
	
	
	
	
	size_t span_subdivision_u;
	size_t span_subdivision_v;

	
	bool skip_mesh_parts;

	uint32_t _end_zero;
} ufbx_tessellate_surface_opts;



typedef struct ufbx_subdivide_opts {
	uint32_t _begin_zero;

	ufbx_allocator_opts temp_allocator;   
	ufbx_allocator_opts result_allocator; 

	ufbx_subdivision_boundary boundary;
	ufbx_subdivision_boundary uv_boundary;

	
	bool ignore_normals;

	
	
	bool interpolate_normals;

	
	bool interpolate_tangents;

	
	
	bool evaluate_source_vertices;

	
	size_t max_source_vertices;

	
	
	bool evaluate_skin_weights;

	
	size_t max_skin_weights;

	
	size_t skin_deformer_index;

	uint32_t _end_zero;
} ufbx_subdivide_opts;



typedef struct ufbx_geometry_cache_opts {
	uint32_t _begin_zero;

	ufbx_allocator_opts temp_allocator;   
	ufbx_allocator_opts result_allocator; 

	
	ufbx_open_file_cb open_file_cb;

	
	double frames_per_second;

	
	ufbx_mirror_axis mirror_axis;

	
	bool use_scale_factor;

	
	ufbx_real scale_factor;

	uint32_t _end_zero;
} ufbx_geometry_cache_opts;



typedef struct ufbx_geometry_cache_data_opts {
	uint32_t _begin_zero;

	
	ufbx_open_file_cb open_file_cb;

	bool additive;
	bool use_weight;
	ufbx_real weight;

	
	bool ignore_transform;

	uint32_t _end_zero;
} ufbx_geometry_cache_data_opts;

typedef struct ufbx_panic {
	bool did_panic;
	size_t message_length;
	char message[UFBX_PANIC_MESSAGE_LENGTH];
} ufbx_panic;



#ifdef __cplusplus
extern "C" {
#endif


ufbx_abi_data const ufbx_string ufbx_empty_string;
ufbx_abi_data const ufbx_blob ufbx_empty_blob;
ufbx_abi_data const ufbx_matrix ufbx_identity_matrix;
ufbx_abi_data const ufbx_transform ufbx_identity_transform;
ufbx_abi_data const ufbx_vec2 ufbx_zero_vec2;
ufbx_abi_data const ufbx_vec3 ufbx_zero_vec3;
ufbx_abi_data const ufbx_vec4 ufbx_zero_vec4;
ufbx_abi_data const ufbx_quat ufbx_identity_quat;


ufbx_abi_data const ufbx_coordinate_axes ufbx_axes_right_handed_y_up;
ufbx_abi_data const ufbx_coordinate_axes ufbx_axes_right_handed_z_up;
ufbx_abi_data const ufbx_coordinate_axes ufbx_axes_left_handed_y_up;
ufbx_abi_data const ufbx_coordinate_axes ufbx_axes_left_handed_z_up;


ufbx_abi_data const size_t ufbx_element_type_size[UFBX_ELEMENT_TYPE_COUNT];


ufbx_abi_data const uint32_t ufbx_source_version;















ufbx_abi bool ufbx_is_thread_safe(void);


ufbx_abi ufbx_scene *ufbx_load_memory(
	const void *data, size_t data_size,
	const ufbx_load_opts *opts, ufbx_error *error);


ufbx_abi ufbx_scene *ufbx_load_file(
	const char *filename,
	const ufbx_load_opts *opts, ufbx_error *error);
ufbx_abi ufbx_scene *ufbx_load_file_len(
	const char *filename, size_t filename_len,
	const ufbx_load_opts *opts, ufbx_error *error);



ufbx_abi ufbx_scene *ufbx_load_stdio(
	void *file,
	const ufbx_load_opts *opts, ufbx_error *error);



ufbx_abi ufbx_scene *ufbx_load_stdio_prefix(
	void *file,
	const void *prefix, size_t prefix_size,
	const ufbx_load_opts *opts, ufbx_error *error);


ufbx_abi ufbx_scene *ufbx_load_stream(
	const ufbx_stream *stream,
	const ufbx_load_opts *opts, ufbx_error *error);


ufbx_abi ufbx_scene *ufbx_load_stream_prefix(
	const ufbx_stream *stream,
	const void *prefix, size_t prefix_size,
	const ufbx_load_opts *opts, ufbx_error *error);


ufbx_abi void ufbx_free_scene(ufbx_scene *scene);


ufbx_abi void ufbx_retain_scene(ufbx_scene *scene);




ufbx_abi size_t ufbx_format_error(char *dst, size_t dst_size, const ufbx_error *error);





ufbx_abi ufbx_prop *ufbx_find_prop_len(const ufbx_props *props, const char *name, size_t name_len);
ufbx_abi ufbx_prop *ufbx_find_prop(const ufbx_props *props, const char *name);




ufbx_abi ufbx_real ufbx_find_real_len(const ufbx_props *props, const char *name, size_t name_len, ufbx_real def);
ufbx_abi ufbx_real ufbx_find_real(const ufbx_props *props, const char *name, ufbx_real def);
ufbx_abi ufbx_vec3 ufbx_find_vec3_len(const ufbx_props *props, const char *name, size_t name_len, ufbx_vec3 def);
ufbx_abi ufbx_vec3 ufbx_find_vec3(const ufbx_props *props, const char *name, ufbx_vec3 def);
ufbx_abi int64_t ufbx_find_int_len(const ufbx_props *props, const char *name, size_t name_len, int64_t def);
ufbx_abi int64_t ufbx_find_int(const ufbx_props *props, const char *name, int64_t def);
ufbx_abi bool ufbx_find_bool_len(const ufbx_props *props, const char *name, size_t name_len, bool def);
ufbx_abi bool ufbx_find_bool(const ufbx_props *props, const char *name, bool def);
ufbx_abi ufbx_string ufbx_find_string_len(const ufbx_props *props, const char *name, size_t name_len, ufbx_string def);
ufbx_abi ufbx_string ufbx_find_string(const ufbx_props *props, const char *name, ufbx_string def);
ufbx_abi ufbx_blob ufbx_find_blob_len(const ufbx_props *props, const char *name, size_t name_len, ufbx_blob def);
ufbx_abi ufbx_blob ufbx_find_blob(const ufbx_props *props, const char *name, ufbx_blob def);


ufbx_abi ufbx_prop *ufbx_find_prop_concat(const ufbx_props *props, const ufbx_string *parts, size_t num_parts);


ufbx_abi ufbx_element *ufbx_get_prop_element(const ufbx_element *element, const ufbx_prop *prop, ufbx_element_type type);


ufbx_abi ufbx_element *ufbx_find_prop_element_len(const ufbx_element *element, const char *name, size_t name_len, ufbx_element_type type);
ufbx_abi ufbx_element *ufbx_find_prop_element(const ufbx_element *element, const char *name, ufbx_element_type type);




ufbx_abi ufbx_element *ufbx_find_element_len(const ufbx_scene *scene, ufbx_element_type type, const char *name, size_t name_len);
ufbx_abi ufbx_element *ufbx_find_element(const ufbx_scene *scene, ufbx_element_type type, const char *name);


ufbx_abi ufbx_node *ufbx_find_node_len(const ufbx_scene *scene, const char *name, size_t name_len);
ufbx_abi ufbx_node *ufbx_find_node(const ufbx_scene *scene, const char *name);


ufbx_abi ufbx_anim_stack *ufbx_find_anim_stack_len(const ufbx_scene *scene, const char *name, size_t name_len);
ufbx_abi ufbx_anim_stack *ufbx_find_anim_stack(const ufbx_scene *scene, const char *name);


ufbx_abi ufbx_material *ufbx_find_material_len(const ufbx_scene *scene, const char *name, size_t name_len);
ufbx_abi ufbx_material *ufbx_find_material(const ufbx_scene *scene, const char *name);



ufbx_abi ufbx_anim_prop *ufbx_find_anim_prop_len(const ufbx_anim_layer *layer, const ufbx_element *element, const char *prop, size_t prop_len);
ufbx_abi ufbx_anim_prop *ufbx_find_anim_prop(const ufbx_anim_layer *layer, const ufbx_element *element, const char *prop);


ufbx_abi ufbx_anim_prop_list ufbx_find_anim_props(const ufbx_anim_layer *layer, const ufbx_element *element);





ufbx_abi ufbx_matrix ufbx_get_compatible_matrix_for_normals(const ufbx_node *node);







ufbx_abi ptrdiff_t ufbx_inflate(void *dst, size_t dst_size, const ufbx_inflate_input *input, ufbx_inflate_retain *retain);



ufbx_abi bool ufbx_default_open_file(void *user, ufbx_stream *stream, const char *path, size_t path_len, const ufbx_open_file_info *info);



ufbx_abi bool ufbx_open_file(ufbx_stream *stream, const char *path, size_t path_len, const ufbx_open_file_opts *opts, ufbx_error *error);
ufbx_unsafe ufbx_abi bool ufbx_open_file_ctx(ufbx_stream *stream, ufbx_open_file_context ctx, const char *path, size_t path_len, const ufbx_open_file_opts *opts, ufbx_error *error);


ufbx_abi bool ufbx_open_memory(ufbx_stream *stream, const void *data, size_t data_size, const ufbx_open_memory_opts *opts, ufbx_error *error);
ufbx_unsafe ufbx_abi bool ufbx_open_memory_ctx(ufbx_stream *stream, ufbx_open_file_context ctx, const void *data, size_t data_size, const ufbx_open_memory_opts *opts, ufbx_error *error);





ufbx_abi ufbx_real ufbx_evaluate_curve(const ufbx_anim_curve *curve, double time, ufbx_real default_value);
ufbx_abi ufbx_real ufbx_evaluate_curve_flags(const ufbx_anim_curve *curve, double time, ufbx_real default_value, uint32_t flags);


ufbx_abi ufbx_real ufbx_evaluate_anim_value_real(const ufbx_anim_value *anim_value, double time);
ufbx_abi ufbx_vec3 ufbx_evaluate_anim_value_vec3(const ufbx_anim_value *anim_value, double time);
ufbx_abi ufbx_real ufbx_evaluate_anim_value_real_flags(const ufbx_anim_value *anim_value, double time, uint32_t flags);
ufbx_abi ufbx_vec3 ufbx_evaluate_anim_value_vec3_flags(const ufbx_anim_value *anim_value, double time, uint32_t flags);



ufbx_abi ufbx_prop ufbx_evaluate_prop_len(const ufbx_anim *anim, const ufbx_element *element, const char *name, size_t name_len, double time);
ufbx_abi ufbx_prop ufbx_evaluate_prop(const ufbx_anim *anim, const ufbx_element *element, const char *name, double time);
ufbx_abi ufbx_prop ufbx_evaluate_prop_flags_len(const ufbx_anim *anim, const ufbx_element *element, const char *name, size_t name_len, double time, uint32_t flags);
ufbx_abi ufbx_prop ufbx_evaluate_prop_flags(const ufbx_anim *anim, const ufbx_element *element, const char *name, double time, uint32_t flags);




ufbx_abi ufbx_props ufbx_evaluate_props(const ufbx_anim *anim, const ufbx_element *element, double time, ufbx_prop *buffer, size_t buffer_size);
ufbx_abi ufbx_props ufbx_evaluate_props_flags(const ufbx_anim *anim, const ufbx_element *element, double time, ufbx_prop *buffer, size_t buffer_size, uint32_t flags);


typedef enum ufbx_transform_flags UFBX_FLAG_REPR {

	
	UFBX_TRANSFORM_FLAG_IGNORE_SCALE_HELPER = 0x1,

	
	
	
	UFBX_TRANSFORM_FLAG_IGNORE_COMPONENTWISE_SCALE = 0x2,

	
	UFBX_TRANSFORM_FLAG_EXPLICIT_INCLUDES = 0x4,

	
	UFBX_TRANSFORM_FLAG_INCLUDE_TRANSLATION = 0x10,
	
	UFBX_TRANSFORM_FLAG_INCLUDE_ROTATION = 0x20,
	
	UFBX_TRANSFORM_FLAG_INCLUDE_SCALE = 0x40,

	
	
	UFBX_TRANSFORM_FLAG_NO_EXTRAPOLATION = 0x80,

	UFBX_FLAG_FORCE_WIDTH(UFBX_TRANSFORM_FLAGS)
} ufbx_transform_flags;




ufbx_abi ufbx_transform ufbx_evaluate_transform(const ufbx_anim *anim, const ufbx_node *node, double time);
ufbx_abi ufbx_transform ufbx_evaluate_transform_flags(const ufbx_anim *anim, const ufbx_node *node, double time, uint32_t flags);



ufbx_abi ufbx_real ufbx_evaluate_blend_weight(const ufbx_anim *anim, const ufbx_blend_channel *channel, double time);
ufbx_abi ufbx_real ufbx_evaluate_blend_weight_flags(const ufbx_anim *anim, const ufbx_blend_channel *channel, double time, uint32_t flags);








ufbx_abi ufbx_scene *ufbx_evaluate_scene(const ufbx_scene *scene, const ufbx_anim *anim, double time, const ufbx_evaluate_opts *opts, ufbx_error *error);





ufbx_abi ufbx_anim *ufbx_create_anim(const ufbx_scene *scene, const ufbx_anim_opts *opts, ufbx_error *error);


ufbx_abi void ufbx_free_anim(ufbx_anim *anim);


ufbx_abi void ufbx_retain_anim(ufbx_anim *anim);





ufbx_abi ufbx_baked_anim *ufbx_bake_anim(const ufbx_scene *scene, const ufbx_anim *anim, const ufbx_bake_opts *opts, ufbx_error *error);

ufbx_abi void ufbx_retain_baked_anim(ufbx_baked_anim *bake);
ufbx_abi void ufbx_free_baked_anim(ufbx_baked_anim *bake);

ufbx_abi ufbx_baked_node *ufbx_find_baked_node_by_typed_id(ufbx_baked_anim *bake, uint32_t typed_id);
ufbx_abi ufbx_baked_node *ufbx_find_baked_node(ufbx_baked_anim *bake, ufbx_node *node);

ufbx_abi ufbx_baked_element *ufbx_find_baked_element_by_element_id(ufbx_baked_anim *bake, uint32_t element_id);
ufbx_abi ufbx_baked_element *ufbx_find_baked_element(ufbx_baked_anim *bake, ufbx_element *element);




ufbx_abi ufbx_vec3 ufbx_evaluate_baked_vec3(ufbx_baked_vec3_list keyframes, double time);




ufbx_abi ufbx_quat ufbx_evaluate_baked_quat(ufbx_baked_quat_list keyframes, double time);





ufbx_abi ufbx_bone_pose *ufbx_get_bone_pose(const ufbx_pose *pose, const ufbx_node *node);




ufbx_abi ufbx_texture *ufbx_find_prop_texture_len(const ufbx_material *material, const char *name, size_t name_len);
ufbx_abi ufbx_texture *ufbx_find_prop_texture(const ufbx_material *material, const char *name);


ufbx_abi ufbx_string ufbx_find_shader_prop_len(const ufbx_shader *shader, const char *name, size_t name_len);
ufbx_abi ufbx_string ufbx_find_shader_prop(const ufbx_shader *shader, const char *name);


ufbx_abi ufbx_shader_prop_binding_list ufbx_find_shader_prop_bindings_len(const ufbx_shader *shader, const char *name, size_t name_len);
ufbx_abi ufbx_shader_prop_binding_list ufbx_find_shader_prop_bindings(const ufbx_shader *shader, const char *name);


ufbx_abi ufbx_shader_texture_input *ufbx_find_shader_texture_input_len(const ufbx_shader_texture *shader, const char *name, size_t name_len);
ufbx_abi ufbx_shader_texture_input *ufbx_find_shader_texture_input(const ufbx_shader_texture *shader, const char *name);




ufbx_abi bool ufbx_coordinate_axes_valid(ufbx_coordinate_axes axes);


ufbx_abi ufbx_vec3 ufbx_vec3_normalize(ufbx_vec3 v);


ufbx_abi ufbx_real ufbx_quat_dot(ufbx_quat a, ufbx_quat b);
ufbx_abi ufbx_quat ufbx_quat_mul(ufbx_quat a, ufbx_quat b);
ufbx_abi ufbx_quat ufbx_quat_normalize(ufbx_quat q);
ufbx_abi ufbx_quat ufbx_quat_fix_antipodal(ufbx_quat q, ufbx_quat reference);
ufbx_abi ufbx_quat ufbx_quat_slerp(ufbx_quat a, ufbx_quat b, ufbx_real t);
ufbx_abi ufbx_vec3 ufbx_quat_rotate_vec3(ufbx_quat q, ufbx_vec3 v);
ufbx_abi ufbx_vec3 ufbx_quat_to_euler(ufbx_quat q, ufbx_rotation_order order);
ufbx_abi ufbx_quat ufbx_euler_to_quat(ufbx_vec3 v, ufbx_rotation_order order);


ufbx_abi ufbx_matrix ufbx_matrix_mul(const ufbx_matrix *a, const ufbx_matrix *b);
ufbx_abi ufbx_real ufbx_matrix_determinant(const ufbx_matrix *m);
ufbx_abi ufbx_matrix ufbx_matrix_invert(const ufbx_matrix *m);





ufbx_abi ufbx_matrix ufbx_matrix_for_normals(const ufbx_matrix *m);


ufbx_abi ufbx_vec3 ufbx_transform_position(const ufbx_matrix *m, ufbx_vec3 v);
ufbx_abi ufbx_vec3 ufbx_transform_direction(const ufbx_matrix *m, ufbx_vec3 v);


ufbx_abi ufbx_matrix ufbx_transform_to_matrix(const ufbx_transform *t);
ufbx_abi ufbx_transform ufbx_matrix_to_transform(const ufbx_matrix *m);





ufbx_abi ufbx_matrix ufbx_catch_get_skin_vertex_matrix(ufbx_panic *panic, const ufbx_skin_deformer *skin, size_t vertex, const ufbx_matrix *fallback);
ufbx_inline ufbx_matrix ufbx_get_skin_vertex_matrix(const ufbx_skin_deformer *skin, size_t vertex, const ufbx_matrix *fallback) {
	return ufbx_catch_get_skin_vertex_matrix(NULL, skin, vertex, fallback);
}



ufbx_abi uint32_t ufbx_get_blend_shape_offset_index(const ufbx_blend_shape *shape, size_t vertex);



ufbx_abi ufbx_vec3 ufbx_get_blend_shape_vertex_offset(const ufbx_blend_shape *shape, size_t vertex);



ufbx_abi ufbx_vec3 ufbx_get_blend_vertex_offset(const ufbx_blend_deformer *blend, size_t vertex);


ufbx_abi void ufbx_add_blend_shape_vertex_offsets(const ufbx_blend_shape *shape, ufbx_vec3 *vertices, size_t num_vertices, ufbx_real weight);



ufbx_abi void ufbx_add_blend_vertex_offsets(const ufbx_blend_deformer *blend, ufbx_vec3 *vertices, size_t num_vertices, ufbx_real weight);




ufbx_abi size_t ufbx_evaluate_nurbs_basis(const ufbx_nurbs_basis *basis, ufbx_real u, ufbx_real *weights, size_t num_weights, ufbx_real *derivatives, size_t num_derivatives);


ufbx_abi ufbx_curve_point ufbx_evaluate_nurbs_curve(const ufbx_nurbs_curve *curve, ufbx_real u);


ufbx_abi ufbx_surface_point ufbx_evaluate_nurbs_surface(const ufbx_nurbs_surface *surface, ufbx_real u, ufbx_real v);


ufbx_abi ufbx_line_curve *ufbx_tessellate_nurbs_curve(const ufbx_nurbs_curve *curve, const ufbx_tessellate_curve_opts *opts, ufbx_error *error);


ufbx_abi ufbx_mesh *ufbx_tessellate_nurbs_surface(const ufbx_nurbs_surface *surface, const ufbx_tessellate_surface_opts *opts, ufbx_error *error);


ufbx_abi void ufbx_free_line_curve(ufbx_line_curve *curve);


ufbx_abi void ufbx_retain_line_curve(ufbx_line_curve *curve);





ufbx_abi uint32_t ufbx_find_face_index(ufbx_mesh *mesh, size_t index);




ufbx_abi uint32_t ufbx_catch_triangulate_face(ufbx_panic *panic, uint32_t *indices, size_t num_indices, const ufbx_mesh *mesh, ufbx_face face);
ufbx_abi uint32_t ufbx_triangulate_face(uint32_t *indices, size_t num_indices, const ufbx_mesh *mesh, ufbx_face face);


ufbx_abi void ufbx_catch_compute_topology(ufbx_panic *panic, const ufbx_mesh *mesh, ufbx_topo_edge *topo, size_t num_topo);
ufbx_abi void ufbx_compute_topology(const ufbx_mesh *mesh, ufbx_topo_edge *topo, size_t num_topo);





ufbx_abi uint32_t ufbx_catch_topo_next_vertex_edge(ufbx_panic *panic, const ufbx_topo_edge *topo, size_t num_topo, uint32_t index);
ufbx_abi uint32_t ufbx_topo_next_vertex_edge(const ufbx_topo_edge *topo, size_t num_topo, uint32_t index);


ufbx_abi uint32_t ufbx_catch_topo_prev_vertex_edge(ufbx_panic *panic, const ufbx_topo_edge *topo, size_t num_topo, uint32_t index);
ufbx_abi uint32_t ufbx_topo_prev_vertex_edge(const ufbx_topo_edge *topo, size_t num_topo, uint32_t index);



ufbx_abi ufbx_vec3 ufbx_catch_get_weighted_face_normal(ufbx_panic *panic, const ufbx_vertex_vec3 *positions, ufbx_face face);
ufbx_abi ufbx_vec3 ufbx_get_weighted_face_normal(const ufbx_vertex_vec3 *positions, ufbx_face face);



ufbx_abi size_t ufbx_catch_generate_normal_mapping(ufbx_panic *panic, const ufbx_mesh *mesh,
	const ufbx_topo_edge *topo, size_t num_topo,
	uint32_t *normal_indices, size_t num_normal_indices, bool assume_smooth);
ufbx_abi size_t ufbx_generate_normal_mapping(const ufbx_mesh *mesh,
	const ufbx_topo_edge *topo, size_t num_topo,
	uint32_t *normal_indices, size_t num_normal_indices, bool assume_smooth);



ufbx_abi void ufbx_catch_compute_normals(ufbx_panic *panic, const ufbx_mesh *mesh, const ufbx_vertex_vec3 *positions,
	const uint32_t *normal_indices, size_t num_normal_indices,
	ufbx_vec3 *normals, size_t num_normals);
ufbx_abi void ufbx_compute_normals(const ufbx_mesh *mesh, const ufbx_vertex_vec3 *positions,
	const uint32_t *normal_indices, size_t num_normal_indices,
	ufbx_vec3 *normals, size_t num_normals);


ufbx_abi ufbx_mesh *ufbx_subdivide_mesh(const ufbx_mesh *mesh, size_t level, const ufbx_subdivide_opts *opts, ufbx_error *error);


ufbx_abi void ufbx_free_mesh(ufbx_mesh *mesh);


ufbx_abi void ufbx_retain_mesh(ufbx_mesh *mesh);






ufbx_abi ufbx_geometry_cache *ufbx_load_geometry_cache(
	const char *filename,
	const ufbx_geometry_cache_opts *opts, ufbx_error *error);
ufbx_abi ufbx_geometry_cache *ufbx_load_geometry_cache_len(
	const char *filename, size_t filename_len,
	const ufbx_geometry_cache_opts *opts, ufbx_error *error);


ufbx_abi void ufbx_free_geometry_cache(ufbx_geometry_cache *cache);

ufbx_abi void ufbx_retain_geometry_cache(ufbx_geometry_cache *cache);


ufbx_abi size_t ufbx_read_geometry_cache_real(const ufbx_cache_frame *frame, ufbx_real *data, size_t num_data, const ufbx_geometry_cache_data_opts *opts);
ufbx_abi size_t ufbx_read_geometry_cache_vec3(const ufbx_cache_frame *frame, ufbx_vec3 *data, size_t num_data, const ufbx_geometry_cache_data_opts *opts);

ufbx_abi size_t ufbx_sample_geometry_cache_real(const ufbx_cache_channel *channel, double time, ufbx_real *data, size_t num_data, const ufbx_geometry_cache_data_opts *opts);
ufbx_abi size_t ufbx_sample_geometry_cache_vec3(const ufbx_cache_channel *channel, double time, ufbx_vec3 *data, size_t num_data, const ufbx_geometry_cache_data_opts *opts);




ufbx_abi ufbx_dom_node *ufbx_dom_find_len(const ufbx_dom_node *parent, const char *name, size_t name_len);
ufbx_abi ufbx_dom_node *ufbx_dom_find(const ufbx_dom_node *parent, const char *name);






ufbx_abi size_t ufbx_generate_indices(const ufbx_vertex_stream *streams, size_t num_streams, uint32_t *indices, size_t num_indices, const ufbx_allocator_opts *allocator, ufbx_error *error);





ufbx_unsafe ufbx_abi void ufbx_thread_pool_run_task(ufbx_thread_pool_context ctx, uint32_t index);



ufbx_unsafe ufbx_abi void ufbx_thread_pool_set_user_ptr(ufbx_thread_pool_context ctx, void *user_ptr);
ufbx_unsafe ufbx_abi void *ufbx_thread_pool_get_user_ptr(ufbx_thread_pool_context ctx);




ufbx_abi ufbx_real ufbx_catch_get_vertex_real(ufbx_panic *panic, const ufbx_vertex_real *v, size_t index);
ufbx_abi ufbx_vec2 ufbx_catch_get_vertex_vec2(ufbx_panic *panic, const ufbx_vertex_vec2 *v, size_t index);
ufbx_abi ufbx_vec3 ufbx_catch_get_vertex_vec3(ufbx_panic *panic, const ufbx_vertex_vec3 *v, size_t index);
ufbx_abi ufbx_vec4 ufbx_catch_get_vertex_vec4(ufbx_panic *panic, const ufbx_vertex_vec4 *v, size_t index);


ufbx_inline ufbx_real ufbx_get_vertex_real(const ufbx_vertex_real *v, size_t index) { ufbx_assert(index < v->indices.count); return v->values.data[(int32_t)v->indices.data[index]]; }
ufbx_inline ufbx_vec2 ufbx_get_vertex_vec2(const ufbx_vertex_vec2 *v, size_t index) { ufbx_assert(index < v->indices.count); return v->values.data[(int32_t)v->indices.data[index]]; }
ufbx_inline ufbx_vec3 ufbx_get_vertex_vec3(const ufbx_vertex_vec3 *v, size_t index) { ufbx_assert(index < v->indices.count); return v->values.data[(int32_t)v->indices.data[index]]; }
ufbx_inline ufbx_vec4 ufbx_get_vertex_vec4(const ufbx_vertex_vec4 *v, size_t index) { ufbx_assert(index < v->indices.count); return v->values.data[(int32_t)v->indices.data[index]]; }

ufbx_abi ufbx_real ufbx_catch_get_vertex_w_vec3(ufbx_panic *panic, const ufbx_vertex_vec3 *v, size_t index);
ufbx_inline ufbx_real ufbx_get_vertex_w_vec3(const ufbx_vertex_vec3 *v, size_t index) { ufbx_assert(index < v->indices.count); return v->values_w.count > 0 ? v->values_w.data[(int32_t)v->indices.data[index]] : 0.0f; }



ufbx_abi ufbx_unknown *ufbx_as_unknown(const ufbx_element *element);
ufbx_abi ufbx_node *ufbx_as_node(const ufbx_element *element);
ufbx_abi ufbx_mesh *ufbx_as_mesh(const ufbx_element *element);
ufbx_abi ufbx_light *ufbx_as_light(const ufbx_element *element);
ufbx_abi ufbx_camera *ufbx_as_camera(const ufbx_element *element);
ufbx_abi ufbx_bone *ufbx_as_bone(const ufbx_element *element);
ufbx_abi ufbx_empty *ufbx_as_empty(const ufbx_element *element);
ufbx_abi ufbx_line_curve *ufbx_as_line_curve(const ufbx_element *element);
ufbx_abi ufbx_nurbs_curve *ufbx_as_nurbs_curve(const ufbx_element *element);
ufbx_abi ufbx_nurbs_surface *ufbx_as_nurbs_surface(const ufbx_element *element);
ufbx_abi ufbx_nurbs_trim_surface *ufbx_as_nurbs_trim_surface(const ufbx_element *element);
ufbx_abi ufbx_nurbs_trim_boundary *ufbx_as_nurbs_trim_boundary(const ufbx_element *element);
ufbx_abi ufbx_procedural_geometry *ufbx_as_procedural_geometry(const ufbx_element *element);
ufbx_abi ufbx_stereo_camera *ufbx_as_stereo_camera(const ufbx_element *element);
ufbx_abi ufbx_camera_switcher *ufbx_as_camera_switcher(const ufbx_element *element);
ufbx_abi ufbx_marker *ufbx_as_marker(const ufbx_element *element);
ufbx_abi ufbx_lod_group *ufbx_as_lod_group(const ufbx_element *element);
ufbx_abi ufbx_skin_deformer *ufbx_as_skin_deformer(const ufbx_element *element);
ufbx_abi ufbx_skin_cluster *ufbx_as_skin_cluster(const ufbx_element *element);
ufbx_abi ufbx_blend_deformer *ufbx_as_blend_deformer(const ufbx_element *element);
ufbx_abi ufbx_blend_channel *ufbx_as_blend_channel(const ufbx_element *element);
ufbx_abi ufbx_blend_shape *ufbx_as_blend_shape(const ufbx_element *element);
ufbx_abi ufbx_cache_deformer *ufbx_as_cache_deformer(const ufbx_element *element);
ufbx_abi ufbx_cache_file *ufbx_as_cache_file(const ufbx_element *element);
ufbx_abi ufbx_material *ufbx_as_material(const ufbx_element *element);
ufbx_abi ufbx_texture *ufbx_as_texture(const ufbx_element *element);
ufbx_abi ufbx_video *ufbx_as_video(const ufbx_element *element);
ufbx_abi ufbx_shader *ufbx_as_shader(const ufbx_element *element);
ufbx_abi ufbx_shader_binding *ufbx_as_shader_binding(const ufbx_element *element);
ufbx_abi ufbx_anim_stack *ufbx_as_anim_stack(const ufbx_element *element);
ufbx_abi ufbx_anim_layer *ufbx_as_anim_layer(const ufbx_element *element);
ufbx_abi ufbx_anim_value *ufbx_as_anim_value(const ufbx_element *element);
ufbx_abi ufbx_anim_curve *ufbx_as_anim_curve(const ufbx_element *element);
ufbx_abi ufbx_display_layer *ufbx_as_display_layer(const ufbx_element *element);
ufbx_abi ufbx_selection_set *ufbx_as_selection_set(const ufbx_element *element);
ufbx_abi ufbx_selection_node *ufbx_as_selection_node(const ufbx_element *element);
ufbx_abi ufbx_character *ufbx_as_character(const ufbx_element *element);
ufbx_abi ufbx_constraint *ufbx_as_constraint(const ufbx_element *element);
ufbx_abi ufbx_audio_layer *ufbx_as_audio_layer(const ufbx_element *element);
ufbx_abi ufbx_audio_clip *ufbx_as_audio_clip(const ufbx_element *element);
ufbx_abi ufbx_pose *ufbx_as_pose(const ufbx_element *element);
ufbx_abi ufbx_metadata_object *ufbx_as_metadata_object(const ufbx_element *element);


ufbx_abi bool ufbx_dom_is_array(const ufbx_dom_node *node);
ufbx_abi size_t ufbx_dom_array_size(const ufbx_dom_node *node);
ufbx_abi ufbx_int32_list ufbx_dom_as_int32_list(const ufbx_dom_node *node);
ufbx_abi ufbx_int64_list ufbx_dom_as_int64_list(const ufbx_dom_node *node);
ufbx_abi ufbx_float_list ufbx_dom_as_float_list(const ufbx_dom_node *node);
ufbx_abi ufbx_double_list ufbx_dom_as_double_list(const ufbx_dom_node *node);
ufbx_abi ufbx_real_list ufbx_dom_as_real_list(const ufbx_dom_node *node);
ufbx_abi ufbx_blob_list ufbx_dom_as_blob_list(const ufbx_dom_node *node);

#ifdef __cplusplus
}
#endif



#if UFBX_CPP11

struct ufbx_string_view {
	const char *data;
	size_t length;

	ufbx_string_view() : data(nullptr), length(0) { }
	ufbx_string_view(const char *data_, size_t length_) : data(data_), length(length_) { }
	UFBX_CONVERSION_TO_IMPL(ufbx_string_view)
};

ufbx_inline ufbx_scene *ufbx_load_file(ufbx_string_view filename, const ufbx_load_opts *opts, ufbx_error *error) { return ufbx_load_file_len(filename.data, filename.length, opts, error); }
ufbx_inline ufbx_prop *ufbx_find_prop(const ufbx_props *props, ufbx_string_view name) { return ufbx_find_prop_len(props, name.data, name.length); }
ufbx_inline ufbx_real ufbx_find_real(const ufbx_props *props, ufbx_string_view name, ufbx_real def) { return ufbx_find_real_len(props, name.data, name.length, def); }
ufbx_inline ufbx_vec3 ufbx_find_vec3(const ufbx_props *props, ufbx_string_view name, ufbx_vec3 def) { return ufbx_find_vec3_len(props, name.data, name.length, def); }
ufbx_inline int64_t ufbx_find_int(const ufbx_props *props, ufbx_string_view name, int64_t def) { return ufbx_find_int_len(props, name.data, name.length, def); }
ufbx_inline bool ufbx_find_bool(const ufbx_props *props, ufbx_string_view name, bool def) { return ufbx_find_bool_len(props, name.data, name.length, def); }
ufbx_inline ufbx_string ufbx_find_string(const ufbx_props *props, ufbx_string_view name, ufbx_string def) { return ufbx_find_string_len(props, name.data, name.length, def); }
ufbx_inline ufbx_blob ufbx_find_blob(const ufbx_props *props, ufbx_string_view name, ufbx_blob def) { return ufbx_find_blob_len(props, name.data, name.length, def); }
ufbx_inline ufbx_element *ufbx_find_prop_element(const ufbx_element *element, ufbx_string_view name, ufbx_element_type type) { return ufbx_find_prop_element_len(element, name.data, name.length, type); }
ufbx_inline ufbx_element *ufbx_find_element(const ufbx_scene *scene, ufbx_element_type type, ufbx_string_view name) { return ufbx_find_element_len(scene, type, name.data, name.length); }
ufbx_inline ufbx_node *ufbx_find_node(const ufbx_scene *scene, ufbx_string_view name) { return ufbx_find_node_len(scene, name.data, name.length); }
ufbx_inline ufbx_anim_stack *ufbx_find_anim_stack(const ufbx_scene *scene, ufbx_string_view name) { return ufbx_find_anim_stack_len(scene, name.data, name.length); }
ufbx_inline ufbx_material *ufbx_find_material(const ufbx_scene *scene, ufbx_string_view name) { return ufbx_find_material_len(scene, name.data, name.length); }
ufbx_inline ufbx_anim_prop *ufbx_find_anim_prop(const ufbx_anim_layer *layer, const ufbx_element *element, ufbx_string_view prop) { return ufbx_find_anim_prop_len(layer, element, prop.data, prop.length); }
ufbx_inline ufbx_prop ufbx_evaluate_prop(const ufbx_anim *anim, const ufbx_element *element, ufbx_string_view name, double time) { return ufbx_evaluate_prop_len(anim, element, name.data, name.length, time); }
ufbx_inline ufbx_texture *ufbx_find_prop_texture(const ufbx_material *material, ufbx_string_view name) { return ufbx_find_prop_texture_len(material, name.data, name.length); }
ufbx_inline ufbx_string ufbx_find_shader_prop(const ufbx_shader *shader, ufbx_string_view name) { return ufbx_find_shader_prop_len(shader, name.data, name.length); }
ufbx_inline ufbx_shader_prop_binding_list ufbx_find_shader_prop_bindings(const ufbx_shader *shader, ufbx_string_view name) { return ufbx_find_shader_prop_bindings_len(shader, name.data, name.length); }
ufbx_inline ufbx_shader_texture_input *ufbx_find_shader_texture_input(const ufbx_shader_texture *shader, ufbx_string_view name) { return ufbx_find_shader_texture_input_len(shader, name.data, name.length); }
ufbx_inline ufbx_geometry_cache *ufbx_load_geometry_cache(ufbx_string_view filename, const ufbx_geometry_cache_opts *opts, ufbx_error *error) { return ufbx_load_geometry_cache_len(filename.data, filename.length, opts, error); }
ufbx_inline ufbx_dom_node *ufbx_dom_find(const ufbx_dom_node *parent, ufbx_string_view name) { return ufbx_dom_find_len(parent, name.data, name.length); }

#endif

#if UFBX_CPP11

template <typename T>
struct ufbx_type_traits { enum { valid = 0 }; };

template<> struct ufbx_type_traits<ufbx_scene> {
	enum { valid = 1 };
	static void retain(ufbx_scene *ptr) { ufbx_retain_scene(ptr); }
	static void free(ufbx_scene *ptr) { ufbx_free_scene(ptr); }
};

template<> struct ufbx_type_traits<ufbx_mesh> {
	enum { valid = 1 };
	static void retain(ufbx_mesh *ptr) { ufbx_retain_mesh(ptr); }
	static void free(ufbx_mesh *ptr) { ufbx_free_mesh(ptr); }
};

template<> struct ufbx_type_traits<ufbx_line_curve> {
	enum { valid = 1 };
	static void retain(ufbx_line_curve *ptr) { ufbx_retain_line_curve(ptr); }
	static void free(ufbx_line_curve *ptr) { ufbx_free_line_curve(ptr); }
};

template<> struct ufbx_type_traits<ufbx_geometry_cache> {
	enum { valid = 1 };
	static void retain(ufbx_geometry_cache *ptr) { ufbx_retain_geometry_cache(ptr); }
	static void free(ufbx_geometry_cache *ptr) { ufbx_free_geometry_cache(ptr); }
};

template<> struct ufbx_type_traits<ufbx_anim> {
	enum { valid = 1 };
	static void retain(ufbx_anim *ptr) { ufbx_retain_anim(ptr); }
	static void free(ufbx_anim *ptr) { ufbx_free_anim(ptr); }
};

template<> struct ufbx_type_traits<ufbx_baked_anim> {
	enum { valid = 1 };
	static void retain(ufbx_baked_anim *ptr) { ufbx_retain_baked_anim(ptr); }
	static void free(ufbx_baked_anim *ptr) { ufbx_free_baked_anim(ptr); }
};

class ufbx_deleter {
public:
	template <typename T>
	void operator()(T *ptr) const {
		static_assert(ufbx_type_traits<T>::valid, "ufbx_deleter() unsupported for type");
		ufbx_type_traits<T>::free(ptr);
	}
};




template <typename T>
class ufbx_unique_ptr {
	T *ptr;
	using traits = ufbx_type_traits<T>;
	static_assert(ufbx_type_traits<T>::valid, "ufbx_unique_ptr unsupported for type");
public:
	ufbx_unique_ptr() noexcept : ptr(nullptr) { }
	explicit ufbx_unique_ptr(T *ptr_) noexcept : ptr(ptr_) { }
	ufbx_unique_ptr(ufbx_unique_ptr &&ref) noexcept : ptr(ref.ptr) { ref.ptr = nullptr; }
	~ufbx_unique_ptr() { traits::free(ptr); }

	ufbx_unique_ptr &operator=(ufbx_unique_ptr &&ref) noexcept {
		if (&ref == this) return *this;
		ptr = ref.ptr;
		ref.ptr = nullptr;
		return *this;
	}

	void reset(T *new_ptr=nullptr) noexcept {
		traits::free(ptr);
		ptr = new_ptr;
	}

	void swap(ufbx_unique_ptr &ref) noexcept {
		T *tmp = ptr;
		ptr = ref.ptr;
		ref.ptr = tmp;
	}

	T &operator*() const noexcept { return *ptr; }
	T *operator->() const noexcept { return ptr; }
	T *get() const noexcept { return ptr; }
	explicit operator bool() const noexcept { return ptr != nullptr; }
};



template <typename T>
class ufbx_shared_ptr {
	T *ptr;
	using traits = ufbx_type_traits<T>;
	static_assert(ufbx_type_traits<T>::valid, "ufbx_shared_ptr unsupported for type");
public:

	ufbx_shared_ptr() noexcept : ptr(nullptr) { }
	explicit ufbx_shared_ptr(T *ptr_) noexcept : ptr(ptr_) { }
	ufbx_shared_ptr(const ufbx_shared_ptr &ref) noexcept : ptr(ref.ptr) { traits::retain(ref.ptr); }
	ufbx_shared_ptr(ufbx_shared_ptr &&ref) noexcept : ptr(ref.ptr) { ref.ptr = nullptr; }
	~ufbx_shared_ptr() { traits::free(ptr); }

	ufbx_shared_ptr &operator=(const ufbx_shared_ptr &ref) noexcept {
		if (&ref == this) return *this;
		traits::free(ptr);
		traits::retain(ref.ptr);
		ptr = ref.ptr;
		return *this;
	}

	ufbx_shared_ptr &operator=(ufbx_shared_ptr &&ref) noexcept {
		if (&ref == this) return *this;
		ptr = ref.ptr;
		ref.ptr = nullptr;
		return *this;
	}

	void reset(T *new_ptr=nullptr) noexcept {
		traits::free(ptr);
		ptr = new_ptr;
	}

	void swap(ufbx_shared_ptr &ref) noexcept {
		T *tmp = ptr;
		ptr = ref.ptr;
		ref.ptr = tmp;
	}

	T &operator*() const noexcept { return *ptr; }
	T *operator->() const noexcept { return ptr; }
	T *get() const noexcept { return ptr; }
	explicit operator bool() const noexcept { return ptr != nullptr; }
};

#endif









#define UFBX_Lcl_Translation "Lcl Translation"




#define UFBX_Lcl_Rotation "Lcl Rotation"



#define UFBX_Lcl_Scaling "Lcl Scaling"



#define UFBX_RotationOrder "RotationOrder"



#define UFBX_ScalingPivot "ScalingPivot"



#define UFBX_RotationPivot "RotationPivot"



#define UFBX_ScalingOffset "ScalingOffset"



#define UFBX_RotationOffset "RotationOffset"




#define UFBX_PreRotation "PreRotation"




#define UFBX_PostRotation "PostRotation"



#define UFBX_Visibility "Visibility"



#define UFBX_Weight "Weight"



#define UFBX_DeformPercent "DeformPercent"

#if defined(_MSC_VER)
	#pragma warning(pop)
#elif defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__)
	#pragma GCC diagnostic pop
#endif

#endif
