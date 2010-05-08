/******************************************************
 * This is the main file for the mips1 ArchC model    *
 * This file is automatically generated by ArchC      *
 * WITHOUT WARRANTY OF ANY KIND, either express       *
 * or implied.                                        *
 * For more information on ArchC, please visit:       *
 * http://www.archc.org                               *
 *                                                    *
 * The ArchC Team                                     *
 * Computer Systems Laboratory (LSC)                  *
 * IC-UNICAMP                                         *
 * http://www.lsc.ic.unicamp.br                       *
 ******************************************************/
 
// Rodolfo editou aqui
//
const char *project_name="mips1";
const char *project_file="mips1.ac";
const char *archc_version="2.0beta1";
const char *archc_options="-abi -dy ";

#include  <systemc.h>
#include  "mips1.H"
#include  "ac_tlm_mem.h"
#include "ac_tlm_cache.h"
#include  "ac_tlm_router.h"
//#include  "ac_tlm_lock.h"
//#include "ac_tlm_mdouble.h"
//#include "ac_tlm_adouble.h"


using user::ac_tlm_mem;
using user::ac_tlm_router;
using user::ac_tlm_cache;
//using user::ac_tlm_lock;
//using user::ac_tlm_mdouble;
//using user::ac_tlm_adouble;

int sc_main(int ac, char *av[])
{

  //!  ISA simulator
  mips1 mips1_proc1("mips1");
  ac_tlm_mem mem("mem");
  //ac_tlm_lock lock("lock");
  //ac_tlm_mdouble mdouble("mdouble");
  //ac_tlm_adouble adouble("adouble");

  // bits of words in blocks - bits of lines - bits of ways
  // for the tests the cache must have 1Kbytes sum=8
  ac_tlm_cache ins_cache("ins_cache", 6, 1, 1, 'i');
  ac_tlm_cache data_cache("data_cache", 3, 3, 2, 'd');

  ac_tlm_router router("router");

#ifdef AC_DEBUG
  ac_trace("mips1_proc1.trace");
#endif 

  mips1_proc1.DM_port(data_cache.target_export);
  mips1_proc1.INSM_port(ins_cache.target_export);

  ins_cache.R_port(router.target_export1);
  data_cache.R_port(router.target_export2);

  router.R_port_mem(mem.target_export);

  //router.R_port_lock(lock.target_export);
  //router.R_port_mdouble(mdouble.target_export);
  //router.R_port_adouble(adouble.target_export);

  mips1_proc1.init(ac, av);
  cerr << endl;

  sc_start();

  mips1_proc1.PrintStat();
  cerr << endl;

#ifdef AC_STATS
  mips1_proc1.ac_sim_stats.time = sc_simulation_time();
  mips1_proc1.ac_sim_stats.print();
#endif 

#ifdef AC_DEBUG
  ac_close_trace();
#endif 

  return mips1_proc1.ac_exit_status;
}
