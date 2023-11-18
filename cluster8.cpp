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

    // 1. Inicializa o envio não-bloqueante para o processo 1
		// buffer: Ponteiro para o local da memória contendo os dados a serem enviados.
		// buffer_count: Número de elementos no buffer.
		// MPI_INT: Tipo de dado dos elementos no buffer (neste caso, inteiros).
		// 1: Rank do processo destino (destino da mensagem).
		// 0: Etiqueta (tag) associada à mensagem. Identifica a natureza da mensagem.
		// MPI_COMM_WORLD: Comunicador que define o grupo de processos envolvidos na comunicação.
		// &request: Ponteiro para uma estrutura MPI_Request, que será usada para verificar ou esperar pela conclusão da operação.
		MPI_Isend(buffer, buffer_count, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
	
    double time_left = 6000.0;

    while (time_left > 0.0) {
      usleep(1000); 
      
      MPI_Test(&request, &request_finished, &status);
      
      time_left -= 1000.0;
    }

    if (!request_finished)
        MPI_Wait(&request, &status);

    for (int i=0; i < buffer_count; ++i)
      buffer[i] = -i;

    MPI_Isend(buffer, buffer_count, MPI_INT, 1, 1, MPI_COMM_WORLD, &request);
    
    time_left = 3000.0;
		
    while (time_left > 0.0) {
      usleep(1000);
      
      MPI_Test(&request, &request_finished, &status);

      time_left -= 1000.0;
    }

    if (!request_finished)
        MPI_Wait(&request, &status);
  }

  else {
    
    sleep(5);

    MPI_Irecv(buffer, buffer_count, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

    MPI_Wait(&request, &status);

    print_buffer();  
    
    sleep(3);

    MPI_Irecv(buffer, buffer_count, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);
    
    MPI_Wait(&request, &status);

    print_buffer();
  }
  

  time += MPI_Wtime();

  double final_time;
  MPI_Reduce(&time, &final_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  
  if (rank == 0)
    std::cout << "Tempo total para o cenário não-bloqueante: " << final_time << "s" << std::endl;
}
