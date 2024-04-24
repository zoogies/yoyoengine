--[[
    This file is a part of yoyoengine. (https://github.com/yoyolick/yoyoengine)
    Copyright (C) 2024  Ryan Zmuda

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
]]

---@class Camera
---@field parent Entity
---@field _c_component lightuserdata
---@field isActive boolean Controls whether the camera is active
---@field isRelative boolean Controls whether the position of the camera is relative to a root transform
---@field z number The z index
---@field x number The x position
---@field y number The y position
---@field w number The width
---@field h number The height
Camera = {
    ---@type Entity
    parent = nil,

    ---@type lightuserdata
    _c_component = nil,

    -- The "fields" dont need listed here
    -- because they are actually stored in
    -- C, and just abstracted through metatable
    -- methods
}

-- define the camera metatable
Camera_mt = {
    __index = function(self, key)
        local parent = rawget(self, "parent")
        if not ValidateEntity(parent) then
            log("error", "Camera field accessed on nil/null entity\n")
            return nil
        end

        -- TODO: IF YOU ADD THIS TYPE OF ACCESS TO ENTITY CLASS THIS WILL OVERFLOW
        local isActive, isRelative, z, x, y, w, h = ye_lua_camera_query(parent._c_entity)

        if key == "isActive" then
            return isActive
        elseif key == "isRelative" then
            return isRelative
        elseif key == "z" then
            return z
        elseif key == "x" then
            return x
        elseif key == "y" then
            return y
        elseif key == "w" then
            return w
        elseif key == "h" then
            return h
        else
            log("error", "Camera field accessed with invalid key\n")
            return nil
        end
    end,

    __newindex = function(self, key, value)
        if not ValidateEntity(self.parent) then
            log("error", "Camera field accessed on nil/null entity\n")
            return
        end

        if key == "isActive" then
            ye_lua_camera_modify(self.parent._c_entity, value, nil, nil, nil, nil, nil, nil)
        elseif key == "isRelative" then
            ye_lua_camera_modify(self.parent._c_entity, nil, value, nil, nil, nil, nil, nil)
        elseif key == "z" then
            ye_lua_camera_modify(self.parent._c_entity, nil, nil, value, nil, nil, nil, nil)
        elseif key == "x" then
            ye_lua_camera_modify(self.parent._c_entity, nil, nil, nil, value, nil, nil, nil)
        elseif key == "y" then
            ye_lua_camera_modify(self.parent._c_entity, nil, nil, nil, nil, value, nil, nil)
        elseif key == "w" then
            ye_lua_camera_modify(self.parent._c_entity, nil, nil, nil, nil, nil, value, nil)
        elseif key == "h" then
            ye_lua_camera_modify(self.parent._c_entity, nil, nil, nil, nil, nil, nil, value)
        else
            log("error", "Camera field accessed with invalid key\n")
            return
        end
    end,
}

---**Create a new camera component.**
---
---@param entity Entity The entity to attach the camera to
---@param x number The x position of the camera
---@param y number The y position of the camera
---@param w number The width of the camera
---@param h number The height of the camera
---@param z number The z index of the camera
---
---@return Camera | nil camera The lua camera object
function Camera:addCamera(entity, x, y, w, h, z)
    -- create the camera itself
    local camera = {}
    setmetatable(camera, Camera_mt)

    -- track onto its parent
    -- camera.parent = entity
    rawset(camera, "parent", entity)

    if x and y and w and h and z then
        ye_lua_create_camera(entity._c_entity, x, y, w, h, z)
    else
        log("error", "Camera:addCamera called with missing parameters\n")
        return nil
    end

    entity.Camera = camera
    -- TODO: rawset

    return camera
end