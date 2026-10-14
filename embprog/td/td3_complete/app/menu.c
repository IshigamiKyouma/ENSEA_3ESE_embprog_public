/*
 * menu.c
 *
 *  Created on: Oct 1, 2026
 *      Author: johann
 */
#include "menu.h"

void menu_entry_toggle(menu_entry_t *e, bsp_t *bsp) {
	*e->led_on = !*e->led_on;
	bsp->led_set(bsp->ctx, e->led_index, *e->led_on);
}
void menu_init(menu_t *m) {
	m->count = 0;
	m->cursor = 0;
}
void menu_add_entry(menu_t *m, menu_entry_t entry) {
	m->entries[m->count++] = entry;
}
void menu_process(menu_t *m, bsp_t *bsp) {
	int32_t delta = bsp->encoder_get_delta(bsp);
	if (delta != 0) {
		m->cursor += (int)delta;
		if (m->cursor < 0)
		{ m->cursor = 0; }
		if (m->cursor >= m->count) { m->cursor = m->count - 1; }
	}
	if (bsp->enc_pressed(bsp)) {
		menu_entry_toggle(&m->entries[m->cursor], bsp);
	}
}

void menu_draw(menu_t *m, bsp_t *bsp) {
	bsp->oled_clear(bsp);
	for (int row = 0; row < m->count; row++) {
		int y = 4 + row * 15;
		bool selected = (row == m->cursor);
		if (selected) {
			bsp->oled_fill_rect(bsp, 0, y, 127, 13, true);
			bsp->oled_draw_string(bsp, 4, y + 2, ">", false);
		}
		char line[20];
		const menu_entry_t *e = &m->entries[row];
		snprintf(line, sizeof(line), "%s: %s", e->label, *e->led_on ? "ON" : "OFF");
		bsp->oled_draw_string(bsp, 16, y + 2, line, !selected);
	}
	bsp->oled_show(bsp);
}
