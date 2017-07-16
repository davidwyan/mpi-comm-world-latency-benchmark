function [ ] = plotGraphTest(fileName,size)
  dataFile = fopen(fileName);

  data = fread(dataFile, [size size], 'double');

  data = data'; %transpose because came from C which is row major
  data = 1- log(data);
  data
  figure
  imagesc(data);
  title('Adjaceny Matrix of MPI Nodes');
  colorbar;
end
