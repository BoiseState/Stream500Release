#include <assert.h>
#include <random>
#include "KronGen.h"

#define INITIATOR_A_NUMERATOR 5700
#define INITIATOR_BC_NUMERATOR 1900
#define INITIATOR_DENOMINATOR 10000


KronGen::KronGen(uint_fast32_t init_seed[5], int init_log_n, uint64_t init_start_edge, uint64_t init_end_edge)
{
  class_id = 0;
  seed = init_seed;
  log_n = init_log_n;
  num_nodes = (uint64_t)1 << log_n;
  start_edge = init_start_edge;
  end_edge = init_end_edge;
  ei = start_edge;
  mrg_seed(&state, seed);

  mrg_state new_state = state;
  mrg_skip(&new_state, 50, 7, 0);
  val0 = mrg_get_uint_orig(&new_state);
  val0 *= UINT64_C(0xFFFFFFFF);
  val0 += mrg_get_uint_orig(&new_state);
  val1 = mrg_get_uint_orig(&new_state);
  val1 *= UINT64_C(0xFFFFFFFF);
  val1 += mrg_get_uint_orig(&new_state);
}

static int generate_4way_bernoulli(mrg_state* st, int level, int nlevels) {
#if SPK_NOISE_LEVEL == 0
  /* Avoid warnings */
  (void)level;
  (void)nlevels;
#endif
  /* Generate a pseudorandom number in the range [0, INITIATOR_DENOMINATOR)
   * without modulo bias. */
  static const uint32_t limit = (UINT32_C(0x7FFFFFFF) % INITIATOR_DENOMINATOR);
  uint32_t val = mrg_get_uint_orig(st);
  if (/* Unlikely */ val < limit) {
    do {
      val = mrg_get_uint_orig(st);
    } while (val < limit);
  }
#if SPK_NOISE_LEVEL == 0
  int spk_noise_factor = 0;
#else
  int spk_noise_factor = 2 * SPK_NOISE_LEVEL * level / nlevels - SPK_NOISE_LEVEL;
#endif
  unsigned int adjusted_bc_numerator = (unsigned int)(INITIATOR_BC_NUMERATOR + spk_noise_factor);
  val %= INITIATOR_DENOMINATOR;
  if (val < adjusted_bc_numerator) return 1;
  val = (uint32_t)(val - adjusted_bc_numerator);
  if (val < adjusted_bc_numerator) return 2;
  val = (uint32_t)(val - adjusted_bc_numerator);
#if SPK_NOISE_LEVEL == 0
  if (val < INITIATOR_A_NUMERATOR) return 0;
#else
  if (val < INITIATOR_A_NUMERATOR * (INITIATOR_DENOMINATOR - 2 * INITIATOR_BC_NUMERATOR) / (INITIATOR_DENOMINATOR - 2 * adjusted_bc_numerator)) return 0;
#endif
#if SPK_NOISE_LEVEL == 0
  /* Avoid warnings */
  (void)level;
  (void)nlevels;
#endif
  return 3;
}

static inline uint64_t bitreverse(uint64_t x) {
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3)
#define USE_GCC_BYTESWAP /* __builtin_bswap* are in 4.3 but not 4.2 */
#endif

#ifdef FAST_64BIT_ARITHMETIC

  /* 64-bit code */
#ifdef USE_GCC_BYTESWAP
  x = __builtin_bswap64(x);
#else
  x = (x >> 32) | (x << 32);
  x = ((x >> 16) & UINT64_C(0x0000FFFF0000FFFF)) | ((x & UINT64_C(0x0000FFFF0000FFFF)) << 16);
  x = ((x >>  8) & UINT64_C(0x00FF00FF00FF00FF)) | ((x & UINT64_C(0x00FF00FF00FF00FF)) <<  8);
#endif
  x = ((x >>  4) & UINT64_C(0x0F0F0F0F0F0F0F0F)) | ((x & UINT64_C(0x0F0F0F0F0F0F0F0F)) <<  4);
  x = ((x >>  2) & UINT64_C(0x3333333333333333)) | ((x & UINT64_C(0x3333333333333333)) <<  2);
  x = ((x >>  1) & UINT64_C(0x5555555555555555)) | ((x & UINT64_C(0x5555555555555555)) <<  1);
  return x;

#else

  /* 32-bit code */
  uint32_t h = (uint32_t)(x >> 32);
  uint32_t l = (uint32_t)(x & UINT32_MAX);
#ifdef USE_GCC_BYTESWAP
  h = __builtin_bswap32(h);
  l = __builtin_bswap32(l);
#else
  h = (h >> 16) | (h << 16);
  l = (l >> 16) | (l << 16);
  h = ((h >> 8) & UINT32_C(0x00FF00FF)) | ((h & UINT32_C(0x00FF00FF)) << 8);
  l = ((l >> 8) & UINT32_C(0x00FF00FF)) | ((l & UINT32_C(0x00FF00FF)) << 8);
#endif
  h = ((h >> 4) & UINT32_C(0x0F0F0F0F)) | ((h & UINT32_C(0x0F0F0F0F)) << 4);
  l = ((l >> 4) & UINT32_C(0x0F0F0F0F)) | ((l & UINT32_C(0x0F0F0F0F)) << 4);
  h = ((h >> 2) & UINT32_C(0x33333333)) | ((h & UINT32_C(0x33333333)) << 2);
  l = ((l >> 2) & UINT32_C(0x33333333)) | ((l & UINT32_C(0x33333333)) << 2);
  h = ((h >> 1) & UINT32_C(0x55555555)) | ((h & UINT32_C(0x55555555)) << 1);
  l = ((l >> 1) & UINT32_C(0x55555555)) | ((l & UINT32_C(0x55555555)) << 1);
  return ((uint64_t)l << 32) | h; /* Swap halves */

#endif
}

static inline int64_t scramble(int64_t v0, int lgN, uint64_t val0, uint64_t val1) {
  uint64_t v = (uint64_t)v0;
  v += val0 + val1;
  v *= (val0 | UINT64_C(0x4519840211493211));
  v = (bitreverse(v) >> (64 - lgN));
  assert ((v >> lgN) == 0);
  v *= (val1 | UINT64_C(0x3050852102C843A5));
  v = (bitreverse(v) >> (64 - lgN));
  assert ((v >> lgN) == 0);
  return (int64_t)v;
}

static void make_one_edge(int64_t nverts, int level, int lgN, mrg_state* st, packed_edge* result, uint64_t val0, uint64_t val1) {
  int64_t base_src = 0, base_tgt = 0;
  while (nverts > 1) {
    int square = generate_4way_bernoulli(st, level, lgN);
    int src_offset = square / 2;
    int tgt_offset = square % 2;
    assert (base_src <= base_tgt);
    if (base_src == base_tgt) {
      /* Clip-and-flip for undirected graph */
      if (src_offset > tgt_offset) {
        int temp = src_offset;
        src_offset = tgt_offset;
        tgt_offset = temp;
      }
    }
    nverts /= 2;
    ++level;
    base_src += nverts * src_offset;
    base_tgt += nverts * tgt_offset;
  }
  write_edge(result,
             scramble(base_src, lgN, val0, val1),
             scramble(base_tgt, lgN, val0, val1));
}

void KronGen::generate_edge(int width, int offset, edge *e)
{
  mrg_state new_state = state;
  mrg_skip(&new_state, 0, (uint64_t)ei, 0);
  packed_edge *p_edge = (packed_edge*)malloc(sizeof(packed_edge));
  make_one_edge(num_nodes, 0, log_n, &new_state, p_edge, val0, val1);

  uint64_t v0 = p_edge->v0;
  uint64_t v1 = p_edge->v1;

  ensure_same_subgraph(width, offset, &v0, &v1, 0);

  e->v0 = v0;
  e->v1 = v1;
  ei++;
  free(p_edge);
}

KronGen::~KronGen()
{}
