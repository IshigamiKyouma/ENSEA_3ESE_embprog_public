#include "dino.h"

#define DINO_JUMP_SPEED 6 /* px/tick initiaux : pic a 6+5+4+3+2+1 = 21 px */
#define DINO_GRAVITY 1    /* px/tick perdus a chaque tick */

void dino_init(dino_t *d) {
	d->state = DINO_RUNNING;
	d->height = 0;
	d->vy = 0;
}

bool dino_jump(dino_t *d) {
	if (d->state != DINO_RUNNING) {
		return false; /* garde : pas de saut en l'air */
	}
	d->state = DINO_JUMPING;
	d->vy = DINO_JUMP_SPEED;
	return true;
}

void dino_update(dino_t *d) {
	if (d->state != DINO_JUMPING) {
		return;
	}

	d->height += d->vy;
	d->vy -= DINO_GRAVITY;

	if (d->height <= 0) { /* garde : retombe au sol */
		d->height = 0;
		d->vy = 0;
		d->state = DINO_RUNNING;
	}
}

void dino_draw(const dino_t *d, bsp_t *bsp, int ground_y, int frame) {
	int top = ground_y - DINO_H - d->height;
	int eye_x = DINO_X+ DINO_W - 3;
	int eye_y = top +2;
	/* --- Magician Hat Dimensions --- */
	int hat_h = 6;      /* Crown height */
	int hat_w = 6;      /* Crown width */
	int brim_w = 10;    /* Brim width */
	int brim_h = 2;     /* Brim height */

	/* Center the hat over the head/snout area */
	int hat_center_x = DINO_X + DINO_W - 4;
	int brim_x = hat_center_x - (brim_w / 2);
	int crown_x = hat_center_x - (hat_w / 2);

	/* 1. Magician Hat */
	/* Brim (wide base sitting directly on top) */
	bsp->oled_fill_rect(bsp, brim_x, top - brim_h, brim_w, brim_h, true);
	/* Crown (cylinder going up) */
	bsp->oled_fill_rect(bsp, crown_x, top - brim_h - hat_h, hat_w, hat_h, true);
	/* Ribbon / Band (contrasting 1px line above the brim) */
	bsp->oled_fill_rect(bsp, crown_x, top - brim_h - 2, hat_w, 1, false);

	bsp->oled_fill_rect(bsp, DINO_X, top, DINO_W, DINO_H, true); //body drawing
	bsp->oled_fill_rect(bsp, eye_x, eye_y, 2, 2, false); //eye drawing
	bsp->oled_fill_rect(bsp, DINO_X+2, top + DINO_H - 4, 4,4, false); // hole in between legs

	/* TODO etape 1 : remplacer ce rectangle par un vrai personnage
	 * (corps, oeil, pattes qui alternent en courant, pattes jointes en
	 * l'air...). Le dino occupe la zone DINO_W x DINO_H a partir de
	 * (DINO_X, top) : restez dedans, la collision (etape 3) en depend. */
	if (d->state != DINO_JUMPING){
		if ((frame/2)%2){
			bsp->oled_fill_rect(bsp, DINO_X,top + DINO_H - 2, 2, 2, false); //left leg up
		}
		else{
			bsp->oled_fill_rect(bsp, DINO_X+DINO_W-2,top + DINO_H - 2, 2, 2, false); //right leg up
		}
	}
}
