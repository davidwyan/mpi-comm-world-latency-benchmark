function [data, dataOrg] = plotGraph(fileName,size)
  dataFile = fopen(fileName);

  data = fread(dataFile, [size size], 'double');

  data = data'; %transpose because came from C which is row major
  dataOrg = data;
  data(data~=0) = 1-log2(data(data~=0));
  maxVal = max(data(:));
  data(data~=0) = maxVal - data(data~=0);
  figure
  imagesc(data);
  title('Adjacency Matrix of MPI Nodes');
  colorbar;
  xlabel('Sending Node');
  ylabel('Receiving Node');
  axis image;
end
