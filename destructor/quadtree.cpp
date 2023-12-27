#include "quadtree.h"

// TODO: cleanup resources

bool need_split(SDL_Surface *surf, SDL_Rect r) {
    Uint32 first_pixel = get_pixel(surf, r.x, r.y);
    for (int y=0; y < r.h; ++y) {
        for (int x=0; x < r.w; ++x) {
            if (get_pixel(surf, r.x + x, r.y + y) != first_pixel) {
                return true;
            }
        }
    }
    return false;
}

// if all pixels aren't the same color, need to subdivide
void insert_pixels(Node *n, SDL_Surface *surf, SDL_Rect r) {
    if(need_split(surf, r)) {
        printf("splitting\n");
        n->children[0] = new Node;
        n->children[0]->rect.x = r.x;
        n->children[0]->rect.y = r.y;
        n->children[0]->rect.w = r.w/2;
        n->children[0]->rect.h = r.h/2;
        insert_pixels(n->children[0], surf, n->children[0]->rect);

        n->children[1] = new Node;
        n->children[1]->rect.x = r.x + r.w/2;
        n->children[1]->rect.y = r.y;
        n->children[1]->rect.w = r.w/2;
        n->children[1]->rect.h = r.h/2;
        insert_pixels(n->children[1], surf, n->children[1]->rect);

        n->children[2] = new Node;
        n->children[2]->rect.x = r.x;
        n->children[2]->rect.y = r.y + r.h/2;
        n->children[2]->rect.w = r.w/2;
        n->children[2]->rect.h = r.h/2;
        insert_pixels(n->children[2], surf, n->children[2]->rect);

        n->children[3] = new Node;
        n->children[3]->rect.x = r.x + r.w/2;
        n->children[3]->rect.y = r.y + r.h/2;
        n->children[3]->rect.w = r.w/2;
        n->children[3]->rect.h = r.h/2;
        insert_pixels(n->children[3], surf, n->children[3]->rect);

        n->count = 4; 
    } else {
        n->pixel = get_pixel(surf, r.x, r.y);
    }
}
