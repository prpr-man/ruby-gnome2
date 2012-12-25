/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rb-gobject-introspection.h"

#define RG_TARGET_NAMESPACE rb_cGIInterfaceInfo
#define SELF(self) (RVAL2GI_INTERFACE_INFO(self))

GType
gi_interface_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIInterfaceInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_n_prerequisites(VALUE self)
{
    GIInterfaceInfo *info;

    info = SELF(self);
    return INT2NUM(g_interface_info_get_n_prerequisites(info));
}

static VALUE
rg_get_prerequisite(VALUE self, VALUE rb_n)
{
    GIInterfaceInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_interface_info_get_prerequisite(info, n));
}

static VALUE
rg_n_properties(VALUE self)
{
    GIInterfaceInfo *info;

    info = SELF(self);
    return INT2NUM(g_interface_info_get_n_properties(info));
}

static VALUE
rg_get_property(VALUE self, VALUE rb_n)
{
    GIInterfaceInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_interface_info_get_property(info, n));
}

static VALUE
rg_n_methods(VALUE self)
{
    GIInterfaceInfo *info;

    info = SELF(self);
    return INT2NUM(g_interface_info_get_n_methods(info));
}

static VALUE
rg_get_method(VALUE self, VALUE rb_n_or_name)
{
    GIInterfaceInfo *info;
    GIFunctionInfo *function_info;

    info = SELF(self);
    if (RB_TYPE_P(rb_n_or_name, T_FIXNUM)) {
        gint n;
        n = NUM2INT(rb_n_or_name);
        function_info = g_interface_info_get_method(info, n);
    } else {
        const char *name;
        name = RVAL2CSTR(rb_n_or_name);
        function_info = g_interface_info_find_method(info, name);
    }

    return GI_BASE_INFO2RVAL_WITH_UNREF(function_info);
}

static VALUE
rg_n_signals(VALUE self)
{
    GIInterfaceInfo *info;

    info = SELF(self);
    return INT2NUM(g_interface_info_get_n_signals(info));
}

static VALUE
rg_get_signal(VALUE self, VALUE rb_n_or_name)
{
    GIInterfaceInfo *info;
    GISignalInfo *signal_info;

    info = SELF(self);
    if (RB_TYPE_P(rb_n_or_name, T_FIXNUM)) {
        gint n;
        n = NUM2INT(rb_n_or_name);
        signal_info = g_interface_info_get_signal(info, n);
    } else {
#ifdef HAVE_G_INTERFACE_INFO_FIND_SIGNAL
        const char *name;
        name = RVAL2CSTR(rb_n_or_name);
        signal_info = g_interface_info_find_signal(info, name);
#else
        rb_raise(rb_eArgError,
                 "g_interface_info_find_signal() is defined "
                 "since GObjectIntrospection 1.34");
#endif
    }

    return GI_BASE_INFO2RVAL_WITH_UNREF(signal_info);
}

void
rb_gi_interface_info_init(VALUE rb_mGI, VALUE rb_cGIRegisteredTypeInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_INTERFACE_INFO, "InterfaceInfo", rb_mGI,
				rb_cGIRegisteredTypeInfo);

    RG_DEF_METHOD(n_prerequisites, 0);
    RG_DEF_METHOD(get_prerequisite, 1);
    RG_DEF_METHOD(n_properties, 0);
    RG_DEF_METHOD(get_property, 1);
    RG_DEF_METHOD(n_methods, 0);
    RG_DEF_METHOD(get_method, 1);
    RG_DEF_METHOD(n_signals, 0);
    RG_DEF_METHOD(get_signal, 1);
}
