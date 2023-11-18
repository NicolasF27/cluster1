void play_non_blocking_scenario() {
  MPI_Request request;
  MPI_Status  status;
  int request_finished = 0;

  for (int i=0; i < buffer_count; ++i)
    buffer[i] = (rank == 0 ? i*2 : 0);

  MPI_Barrier(MPI_COMM_WORLD);

  double time = -MPI_Wtime();

  
  if (rank == 0) {
    sleep(3);

    double time_left = 6000.0;
    while (time_left > 0.0) {
      usleep(1000); // We work for 1ms

      time_left -= 1000.0;
    }


    for (int i=0; i < buffer_count; ++i)
      buffer[i] = -i;
    
    time_left = 3000.0;
    while (time_left > 0.0) {
      usleep(1000); 

      time_left -= 1000.0;
    }
    
  }
  else {
    sleep(5);

    print_buffer();
    
    sleep(3);

    print_buffer();
  }

  time += MPI_Wtime();


  double final_time;
  MPI_Reduce(&time, &final_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  
  if (rank == 0)
    std::cout << "Total time for non-blocking scenario : " << final_time << "s" << std::endl;
}
