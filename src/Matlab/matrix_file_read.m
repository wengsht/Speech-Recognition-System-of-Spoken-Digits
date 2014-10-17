function [y] = matrix_file_read(filename)
    fID = fopen(filename);
    if fID < 0
        error ('open ')
    end
    n = 1
    fscanf(fID, '%d', n);

    y = fscanf(fID, '%f', [n inf]);

