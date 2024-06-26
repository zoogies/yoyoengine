/*
    This file is a part of yoyoengine. (https://github.com/zoogies/yoyoengine)
    Copyright (C) 2024  Ryan Zmuda

    Licensed under the MIT license. See LICENSE file in the project root for details.
*/

#include <stdbool.h>

#include <yoyoengine/debug_renderer.h>
#include <yoyoengine/ecs/ecs.h>
#include <yoyoengine/utils.h>

#include "editor.h"
#include "editor_input.h"
#include "editor_ui.h"
#include "editor_selection.h"

bool is_dragging = false;
SDL_Point drag_start;

bool editor_draw_drag_rect = false;

struct editor_selection_node * editor_selections = NULL;
int num_editor_selections = 0;

void editor_deselect_all(){
    ye_reset_editor_selection_group();

    struct editor_selection_node * itr = editor_selections;
    while(itr != NULL){
        struct editor_selection_node * temp = itr;
        itr = itr->next;
        free(temp);
    }
    editor_selections = NULL;
    num_editor_selections = 0;
}

bool already_selected(struct ye_entity * ent){
    struct editor_selection_node * itr = editor_selections;
    while(itr != NULL){
        if(itr->ent == ent) return true;
        itr = itr->next;
    }
    return false;
}

void add_selection(struct ye_entity * ent){
    if(already_selected(ent)) return;

    ye_reset_editor_selection_group();

    // discard selections of editor entities, like editor camera, origin, etc
    if(ent == editor_camera || ent == origin) return;

    struct editor_selection_node * new_node = malloc(sizeof(struct editor_selection_node));
    new_node->ent = ent;
    new_node->next = editor_selections;
    editor_selections = new_node;
    num_editor_selections++;
}

/*
    Only works with entities having transform components
*/
void select_within(SDL_Rect zone){

    /*
        Check if editor selection rect is big
        enough to be considered a selection
    */
    if(abs(zone.w) < PREFS.min_select_px && abs(zone.h) < PREFS.min_select_px){
        return;
    }

    /*
        If w or h is negative, we need to normalize the rect
    */
    if(zone.w < 0){ zone.x += zone.w; zone.w = abs(zone.w); }
    if(zone.h < 0){ zone.y += zone.h; zone.h = abs(zone.h); }

    struct ye_entity_node * itr = transform_list_head;
    while(itr != NULL){
        struct ye_entity * ent = itr->entity;
        struct ye_rectf pos = ye_get_position(ent, YE_COMPONENT_TRANSFORM);
        if(pos.x > zone.x && pos.y > zone.y &&
            pos.x + pos.w < zone.x + zone.w &&
            pos.y + pos.h < zone.y + zone.h){
            add_selection(ent);
        }
        itr = itr->next;
    }
}

void editor_selection_handler(SDL_Event event){
    // check if mouse left window
    if(event.type == SDL_WINDOWEVENT){
        if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
            is_dragging = false; editor_draw_drag_rect = false;
        }
    }

    // if we arent hovering editor ignore
    int mx, my; SDL_GetMouseState(&mx, &my);
    if(!is_hovering_editor(mx, my) || lock_viewport_interaction) {
        is_dragging = false; editor_draw_drag_rect = false;
        return;
    }

    // update mx and my to be world positions
    // my = my - 35; // account for the menu bar

    float scaleX = (float)YE_STATE.engine.screen_width / (float)YE_STATE.engine.target_camera->camera->view_field.w;
    float scaleY = (float)YE_STATE.engine.screen_height / (float)YE_STATE.engine.target_camera->camera->view_field.h;
    struct ye_rectf campos = ye_get_position(YE_STATE.engine.target_camera, YE_COMPONENT_CAMERA);
    mx = ((mx / scaleX) + campos.x);
    my = ((my / scaleY) + campos.y);

    if(is_dragging)
        editor_draw_drag_rect = !(abs(mx - drag_start.x) < PREFS.min_select_px && abs(my - drag_start.y) < PREFS.min_select_px);
    

    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                // if we clicked at all and werent holding ctrl, clear selections
                if (!(SDL_GetModState() & KMOD_CTRL)) {
                    editor_deselect_all();
                }
                
                /*
                    Detect the item clicked on and add it to the selected list

                    We check for:
                    - clicked within renderer comp bounds
                    - clicked within collider bounds
                    - clicked within audiosource bounds
                    - clicked within camera viewport
                    // (not now) - clicked within 10px of transform position
                */
                struct ye_entity_node *itr = entity_list_head;
                while(itr != NULL){
                    struct ye_entity * ent = itr->entity;
                    struct ye_rectf pos;
                    if(ye_component_exists(ent, YE_COMPONENT_RENDERER)){
                        pos = ye_get_position(ent, YE_COMPONENT_RENDERER);
                    }
                    else if(ye_component_exists(ent, YE_COMPONENT_COLLIDER)){
                        pos = ye_get_position(ent, YE_COMPONENT_COLLIDER);
                    }
                    else if(ye_component_exists(ent, YE_COMPONENT_AUDIOSOURCE)){
                        pos = ye_get_position(ent, YE_COMPONENT_AUDIOSOURCE);
                    }
                    else if(ye_component_exists(ent, YE_COMPONENT_CAMERA)){
                        pos = ye_get_position(ent, YE_COMPONENT_CAMERA);
                    }
                    else if(ye_component_exists(ent, YE_COMPONENT_TRANSFORM)){
                        pos = ye_get_position(ent, YE_COMPONENT_TRANSFORM);
                    }
                    else{
                        itr = itr->next;
                        continue;
                    }

                    if(mx > pos.x && my > pos.y &&
                        mx < pos.x + pos.w && my < pos.y + pos.h){
                        add_selection(ent);
                        break;
                    }

                    itr = itr->next;
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (is_dragging) {
                    is_dragging = false;
                    editor_draw_drag_rect = false;

                    // attempt to select all items within the drag rectangle
                    select_within(editor_selecting_rect);
                }
            }
            break;
        case SDL_MOUSEMOTION:
            if (event.motion.state & SDL_BUTTON_LMASK) {
                if (!is_dragging) {
                    // Start dragging if not already dragging
                    is_dragging = true;

                    // Set the start point of the drag rectangle
                    drag_start = (SDL_Point){mx, my};
                }

                editor_selecting_rect = (SDL_Rect){drag_start.x, drag_start.y, mx - drag_start.x, my - drag_start.y};
            }
            break;
        default:
            break;
    }
}

/*
    TODO: honestly, for selected entities we should always display their
    collider and audiosource bounds, and a box around renderer area if invisible.
    The actual rect that surrounds them all could show the minimum rect area to encompass
    all of them.
*/
void editor_render_selection_rects(){
    struct editor_selection_node * itr = editor_selections;
    while(itr != NULL){
        /*
            Do some checks to show the "area" of selected entities. We can only show
            entities that have some kind of "bound" ie: renderer, collider, audio source.
            The shape we draw when selected takes precedence in this order (high to low):
            - renderer bounds
            - collider bounds
            - audio source bounds
            - camera viewport bounds
            - if none of these components exist, draw a dot at the center of the entity transform.
            - if a transform doesnt exist, skip the entity
        */
        struct ye_entity * ent = itr->ent;

        SDL_Color select_color = (SDL_Color){255, 0, 255, 255};

        if(ye_component_exists(ent, YE_COMPONENT_RENDERER)){
            struct ye_rectf pos = ye_get_position(ent, YE_COMPONENT_RENDERER);
            ye_debug_render_rect(pos.x, pos.y, pos.w, pos.h, select_color, 8);
        }
        else if(ye_component_exists(ent, YE_COMPONENT_COLLIDER)){
            struct ye_rectf pos = ye_get_position(ent, YE_COMPONENT_COLLIDER);
            ye_debug_render_rect(pos.x, pos.y, pos.w, pos.h, select_color, 8);
        }
        else if(ye_component_exists(ent, YE_COMPONENT_AUDIOSOURCE)){
            struct ye_rectf pos = ye_get_position(ent, YE_COMPONENT_AUDIOSOURCE);
            ye_debug_render_rect(pos.x, pos.y, pos.w, pos.h, select_color, 8);
        }
        else if(ye_component_exists(ent, YE_COMPONENT_CAMERA)){
            struct ye_rectf pos = ye_get_position(ent, YE_COMPONENT_CAMERA);
            ye_debug_render_rect(pos.x, pos.y, pos.w, pos.h, select_color, 8);
        }
        else if(ye_component_exists(ent, YE_COMPONENT_TRANSFORM)){
            struct ye_rectf pos = ye_get_position(ent, YE_COMPONENT_TRANSFORM);
            ye_debug_render_rect(pos.x - 5, pos.y - 5, 10, 10, select_color, 8);
        }

        itr = itr->next;
    }
}

bool editor_is_selected(struct ye_entity * ent){
    struct editor_selection_node * itr = editor_selections;
    while(itr != NULL){
        if(itr->ent == ent) return true;
        itr = itr->next;
    }
    return false;
}

void editor_deselect(struct ye_entity * ent){
    ye_reset_editor_selection_group();
    
    struct editor_selection_node * itr = editor_selections;
    struct editor_selection_node * prev = NULL;
    while(itr != NULL){
        if(itr->ent == ent){
            if(prev == NULL){
                editor_selections = itr->next;
            }
            else{
                prev->next = itr->next;
            }
            free(itr);
            num_editor_selections--;
            return;
        }
        prev = itr;
        itr = itr->next;
    }
}

void editor_select(struct ye_entity * ent){
    ye_reset_editor_selection_group();

    // check if keyboard is currently pressing ctrl
    if(!(SDL_GetModState() & KMOD_CTRL)){
        editor_deselect_all();
    }
    add_selection(ent);
}