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
#include  "ac_tlm_lock.h"
#include  "ac_tlm_router.h"

using user::ac_tlm_mem;
using user::ac_tlm_lock;
using user::ac_tlm_router;

#define NPROC 2

int sc_main(int ac, char *av[])
{
  char *param[NPROC][4];
  
  for (int i=0; i < NPROC; i++)
  {
    for (int j=0; j < 2; j++)
    {
      param[i][j] = (char*)malloc(128);
      strcpy(param[i][j], av[j]);
    }
  }

 //!  ISA simulator
  mips1 mips1_proc1("mips1_1");
  mips1 mips1_proc2("mips1_2");
  ac_tlm_mem mem("mem");
  ac_tlm_lock lock("lock");
  ac_tlm_router router("router");

#ifdef AC_DEBUG
  ac_trace("mips1_proc1.trace");
#endif 

  mips1_proc1.DM_port(router.target_export1);
  mips1_proc2.DM_port(router.target_export2);

  router.R_port_mem(mem.target_export);
  router.R_port_lock(lock.target_export);

  
  mips1_proc1.init(ac, (char**)param[0]);
  mips1_proc2.init(ac, (char**)param[1]);

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
