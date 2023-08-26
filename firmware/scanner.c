#include "scanner.h"

#include "adc.h"
#include "frame.h"
#include "mux.h"
#include "timer.h"

#include <stdlib.h>

// TODO refactor, build proper state machine.

static enum {
  PENDING,
  SELECTING_COLUMN,
  SELECTING_ROW,
  SCANNING,
} _state = PENDING;

static size_t ctr = 0;
static size_t row = 0;
static size_t col = 0;

static void _tick() {
  switch (_state) {
  case PENDING:
    if (!ctr) {
      _state = SELECTING_COLUMN;
    }
    break;
  case SELECTING_COLUMN:
    mux_select(col);
    _state = SELECTING_ROW;
    break;
  case SELECTING_ROW:
    adc_select(row);
    _state = SCANNING;
    break;
  case SCANNING:
    adc_read(frame + row * WIDTH + col);
    col += 1;
    col %= WIDTH;
    if (!col) {
      row += 1;
      row %= HEIGHT;
      if (!row) {
        _state = PENDING;
        frame_ready = true;
      } else {
        _state = SELECTING_ROW;
      }
    }
    else {
        _state = SELECTING_COLUMN;
    }
    break;
  }
  ctr += 1;
  ctr %= 100;
}

void scanner_init() { timer_handler = _tick; }
