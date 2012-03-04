#include "lua_pub.h"

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_buf.h>

#define LUA_FILENAME "req_route.lua"

int get_route_id(ngx_log_t *log, int method, char* uri, char* args, char* body, int body_size)
{
    if (method)
    {
        ngx_log_debug(NGX_LOG_DEBUG_HTTP, log, 0, "method:%d", method);
    }
    if (uri)
    {
        ngx_log_debug(NGX_LOG_DEBUG_HTTP, log, 0, "uri:%s", uri);
    }
    if (args)
    {
        ngx_log_debug(NGX_LOG_DEBUG_HTTP, log, 0, "args:%s", args);
    }
    if (body)
    {
        ngx_log_debug(NGX_LOG_DEBUG_HTTP, log, 0, "body:%s", body);
    }

    const char lua_funcname[] = "get_route_id";

    lua_State *L = luaL_newstate();

    luaL_openlibs(L);

    if (luaL_loadfile(L, LUA_FILENAME) || lua_pcall(L, 0, 0, 0))
    {
        ngx_log_error(NGX_LOG_ERR, log, 0, "cannot run configuration file: %s", lua_tostring(L, -1));
        lua_close(L);
        return -1;
    }

    lua_getglobal(L, lua_funcname); /* function to be called */
    lua_pushnumber(L, method);
    lua_pushstring(L, uri);
    lua_pushstring(L, args);
    lua_pushlstring(L, body, body_size);

    /* do the call (1 arguments, 1 result) */
    if (lua_pcall(L, 4, 1, 0) != 0)
    {
        ngx_log_error(NGX_LOG_ERR, log, 0, "error running function %s: %s", lua_funcname, lua_tostring(L, -1));
        lua_close(L);
        return -2;
    }

    /* retrieve result */
    if (!lua_isnumber(L, -1))
    {
        ngx_log_error(NGX_LOG_ERR, log, 0, "function %s must return a number", lua_funcname);
        lua_close(L);
        return -3;
    }

    int result = (int)lua_tonumber(L, -1);

    lua_pop(L, 1); /* pop returned value */

    lua_close(L);
    return result;

}
