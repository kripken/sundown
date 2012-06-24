/*
 * Copyright (c) 2011, Vicent Marti
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "markdown.h"
#include "html.h"
#include "buffer.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// http://drj11.wordpress.com/2007/04/08/sizeofchar-is-1/
char *str_to_html(const char *in) {
  struct buf *ob;

  struct sd_callbacks callbacks;
  struct html_renderopt options;
  struct sd_markdown *markdown;

  // Use 2048 for block size.
  ob = bufnew(2048);

  sdhtml_renderer(&callbacks, &options, 0);
  /* Enable all extensions. */
  markdown = sd_markdown_new(MKDEXT_NO_INTRA_EMPHASIS | MKDEXT_TABLES | MKDEXT_FENCED_CODE | MKDEXT_AUTOLINK | MKDEXT_STRIKETHROUGH | MKDEXT_SPACE_HEADERS | MKDEXT_SUPERSCRIPT | MKDEXT_LAX_SPACING, 16, &callbacks, &options);

  // Do not strlen + 1 or it will render incomplete markup.
  sd_markdown_render(ob, (uint8_t*) in, strlen(in), markdown);
  sd_markdown_free(markdown);

  // +1 for null termination of string.
  char* html = malloc(ob->size + 1);
  strcpy(html, bufcstr(ob));

  /* cleanup */  
  bufrelease(ob);

  return html;
}
