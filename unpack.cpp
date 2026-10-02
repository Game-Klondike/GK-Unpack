// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * gk-unpack — decompresses one Oodle Kraken stream with ooz (https://github.com/powzix/ooz).
 *
 *   gk-unpack <unpacked size>  < compressed stream  > unpacked bytes
 *
 * stdin and stdout are binary. Exit code 0 = done; 1 = bad arguments or input (the reason goes to stderr).
 * It knows nothing about any game or file format: bytes in, bytes out.
 *
 * Copyright (C) 2026, Game Klondike
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any
 * later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
 * License for more details. You should have received a copy of the GNU General Public License along with this
 * program. If not, see <https://www.gnu.org/licenses/>.
 */
#define main ooz_main // ooz's own command-line program is not used
#include "kraken.cpp"   // ooz, unchanged (the build puts the ooz folder on the include path)
#undef main

#include <fcntl.h>
#include <io.h>
#include <vector>

// ooz's Kraken_Decompress, except that bytes after the end of the stream are ignored, as the official
// OodleLZ_Decompress does: some writers pad the stream, and ooz would reject such input.
static int Unpack(const byte *src, size_t src_len, byte *dst, size_t dst_len) {
  KrakenDecoder *dec = Kraken_Create();
  int offset = 0;
  while (dst_len != 0) {
    if (!Kraken_DecodeStep(dec, dst, offset, dst_len, src, src_len) || dec->src_used == 0) {
      Kraken_Destroy(dec);
      return -1;
    }
    src += dec->src_used;
    src_len -= dec->src_used;
    dst_len -= dec->dst_used;
    offset += dec->dst_used;
  }
  Kraken_Destroy(dec);
  return offset;
}

static int fail(const char *why) { fprintf(stderr, "gk-unpack: %s\n", why); return 1; }

int main(int argc, char **argv) {
  if (argc != 2) return fail("usage: gk-unpack <unpacked size> < input > output");
  char *end = nullptr;
  unsigned long long size = strtoull(argv[1], &end, 10);
  if (!end || *end || size == 0 || size > (1ull << 30)) return fail("bad unpacked size");

  _setmode(_fileno(stdin), _O_BINARY);
  _setmode(_fileno(stdout), _O_BINARY);

  std::vector<unsigned char> in;
  unsigned char buf[1 << 16];
  size_t n;
  while ((n = fread(buf, 1, sizeof buf, stdin)) > 0) in.insert(in.end(), buf, buf + n);
  if (in.empty()) return fail("no input");

  std::vector<unsigned char> out(size + 64); // ooz may write up to 64 bytes past the end (its SAFE_SPACE)
  int got = Unpack(in.data(), in.size(), out.data(), (size_t)size);
  if (got < 0 || (unsigned long long)got != size) return fail("decompress error");
  if (fwrite(out.data(), 1, (size_t)size, stdout) != size) return fail("write error");
  return 0;
}
