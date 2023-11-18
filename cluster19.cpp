void compute(int total_count, int my_count, float my_points[][3]) {
  // total_count é o número total de pontos
  // my_count é o número de pontos para este processo
  // my_points é uma matriz de float com formato [my_count][3]

  // 1- Soma de todos os pontos em local_sum
  float local_sum[3] = {0.0f, 0.0f, 0.0f};
  for (int i = 0; i < my_count; ++i) {
    for (int j = 0; j < 3; ++j) {
      local_sum[j] += my_points[i][j];
    }
  }

  // 2- Reduzir a soma de todos os pontos na variável barycentre
  float barycentre[3];
  MPI_Allreduce(local_sum, barycentre, 3, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

  // 3- Dividir cada componente do barycentre pelo número total de pontos
  for (int i = 0; i < 3; ++i) {
    barycentre[i] /= total_count;
  }

  // Para cada ponto
  for (int i = 0; i < my_count; ++i) {
    float dist = 0.0f;

    // 4- Calcular a distância para cada ponto
    for (int j = 0; j < 3; ++j) {
      float diff = my_points[i][j] - barycentre[j];
      dist += diff * diff;
    }
    dist = sqrt(dist);

    // E imprimir o resultado
    std::cout << rank << " " << dist << std::endl;
  }
}
