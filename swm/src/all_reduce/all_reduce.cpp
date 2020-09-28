#include "all_reduce.h"

AllReduceSWMUserCode::AllReduceSWMUserCode(
    boost::property_tree::ptree cfg,
    void**& generic_ptrs
) :
    process_cnt(cfg.get<uint32_t>("jobs.size", 1)),
    iteration_cnt(cfg.get<uint32_t>("jobs.cfg.iteration_cnt", 1)),
    msg_req_bytes(cfg.get<uint32_t>("jobs.cfg.msg_req_bytes", 1024)),
    msg_rsp_bytes(cfg.get<uint32_t>("jobs.cfg.msg_rsp_bytes", 0)),
    compute_delay(cfg.get<uint32_t>("jobs.cfg.compute_delay", 0))
{

    request_vc = 0;
    response_vc = 0;

    process_id = *((int*)generic_ptrs[0]);
}

void
AllReduceSWMUserCode::call()
{
	if(process_id == 0)
 	{
		std::cout << std::endl << "JOB: Allreduce | size: " << process_cnt;
		std::cout << " | interation_cnt: " << iteration_cnt;
		std::cout << " | compute_delay: " << compute_delay << std::endl;
	}


    uint32_t tag = 0;
    for(uint32_t iter=0; iter < iteration_cnt; iter++)
    {

	if (compute_delay)
		SWM_Compute(compute_delay);

	if(process_id == 0)
	{
    		SWM_Mark_Iteration(tag);
		tag = tag +1;
	}

	SWM_Allreduce(
		msg_req_bytes, // payload
		msg_rsp_bytes, // pkt_rsp_bytes
		SWM_COMM_WORLD, 
		request_vc,
		response_vc,
		NO_BUFFER,
		NO_BUFFER);

	if(process_id == 0)
	{
    		SWM_Mark_Iteration(tag);        
		tag = tag +1;
	}
    }



    SWM_Finalize();
}




/*




		uint32_t count = 0;
                uint32_t iter_offset = (process_cnt * (iter) );
                SWM_TAG this_tag = SWM_APP_TAG_BASE + (sizeof(SWM_TAG) * ( (process_id + 1) + iter_offset) ); //(iter+1) );
	
		if(debug)
		  {
		    std::cout  << std::endl << "process_id: " << process_id << " expecting to recv data from: " << src_rank_id << " with recv tag: " << this_tag << " | iter_" << iter;
		  }
		

		if(!blocking_comm)
		  {
		    SWM_Irecv(
			      src_rank_id,
			      SWM_COMM_WORLD,
			      this_tag,
			      NO_BUFFER,
			      &(recv_handles[count])
			      );
		  }
		else
		  {
		    SWM_Recv(
			     src_rank_id,
			     SWM_COMM_WORLD,
			     this_tag,
			     NO_BUFFER
			     );
		  }

            if(!blocking_comm)
              {
                SWM_Waitall(recv_limit, recv_handles);
              }


		if(debug)
		  {
		    std::cout << std::endl << "process_id: " << process_id << " received data from src: " << src_rank_id << ", iteration: " << iter ;
		  }

		SWM_Mark_Iteration(iter);        
        } // end-for(iteration_cnt)
    }

        // need to send to everybody every iteration...
        for(uint32_t iter = 0; iter < iteration_cnt; iter++)
          {
        	uint32_t send_count = 0;
            for(uint32_t index = min_dst_id; index <= max_dst_id; index++, send_count++)
              {
                
		uint32_t iter_offset = (process_cnt * (iter) );
		//SWM_TAG this_tag = SWM_APP_TAG_BASE + (sizeof(SWM_TAG) * (index + 1) * (iter+1) );
		SWM_TAG this_tag = SWM_APP_TAG_BASE + (sizeof(SWM_TAG) * ( (index + 1) + iter_offset) );

		uint32_t receive_from_proc = (!use_any_src) ? index : -1;
                //uint32_t send_handle[send_limit];

                if(!blocking_comm)
                  {

                    SWM_Isend(
                              index,
                              SWM_COMM_WORLD,
                              this_tag,
			      -1, 
			      -1,
                              NO_BUFFER,
			      msg_req_bytes, 
			      msg_rsp_bytes,
                              &(send_handles[send_count]),
                              0,
                              0
                              );
                  }
                else
                  {
                    SWM_Send(
                             index,
                             SWM_COMM_WORLD,
                             this_tag,		
			     -1,// req-vc
			     -1, //resp-vc
                             NO_BUFFER,
                             msg_req_bytes, //req-bytes
                             msg_rsp_bytes, //resp-bytes
                             0,//routing type
                             0 //routing type
                             );
                  }


                if(debug)
                  {
                    std::cout << std::endl << "process_id: " << process_id << " sent message to destination: " << index << ", tag: " << this_tag << ", iter: " << iter ;
                  }

		    if (compute_delay)
			SWM_Compute(compute_delay);

              } // end of for-loop(all_destinations)
		if(!blocking_comm)
		  {
		    SWM_Waitall(send_limit, send_handles);
		  }
		SWM_Mark_Iteration(iter);        

          } // end for-loop(iteration_cnt)

      } // end of else if(synchronous && (process_id == dst_rank_id) )
*/

