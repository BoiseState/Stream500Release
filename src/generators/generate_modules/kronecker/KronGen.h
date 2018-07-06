#include "../GeneratorObj.h"
#include "user_settings.h"
#include "splittable_mrg.h"
#include "graph_generator.h"


class KronGen : public GeneratorObj
{
  private:
    uint_fast32_t *seed;
    int log_n;
    uint64_t start_edge;
    uint64_t end_edge;
    uint64_t ei;
    mrg_state state;
    uint64_t val0, val1; /* Values for scrambling */


  public:
    KronGen(uint_fast32_t[5], int init_log_n, uint64_t init_start_edge, uint64_t init_end_edge);
    void generate_edge(int width, int offset, edge *e);
    ~KronGen();
};
