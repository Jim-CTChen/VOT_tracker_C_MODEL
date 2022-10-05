#include <iostream>
#include <string>

// parameter
#include "../inc/state.h"

// function
#include "../inc/controller.h"
#include "../inc/tool.h"

// struct
#include "../inc/specs.h"

int main()
{
  // construct controller
  int num_of_PE_set = 7;
  int num_of_PE_in_set = 3;

  Controller controller(num_of_PE_set, num_of_PE_in_set);

  // initialize convolution config
  InitConvConfig();

  // write weight into DRAM
  std::cout << "Initializing weight..." << std::endl;
  std::string weight_filename = "./test/weight_alexnet33v1.txt";
  controller.InitDramOA(weight_filename);

  // controller.ImgZAlexnet();
  // controller.ImgXAlexnet();
  controller.Init();
  controller.Track();
  std::cout << "ending program" << std::endl;
  return 0;

  specs::RawData input_img;

  int state = S_IDLE;
  bool end_tracking = false;

  // switch (state)
  // {
  // case S_IDLE:
  //   // receive input_target
  //   // input_img = target;
  //   state = S_INIT;
  //   break;
  // case S_INIT:
  //   controller.Init(input_img);
  //   return 0; // FIXME:
  //   break;
  
  // case S_TRACK:
  //   // receive input_search
  //   // input_img = search;
  //   controller.Track(input_img);

  //   // end track when user interrupt
  //   // if (interrupt) end_tracking = True
  //   if (end_tracking) state = S_END;
  //   break;

  // case S_END:
  //   controller.Clear();
  //   state = S_IDLE;
  
  // default:
  //   break;
  // }
    
    

}

