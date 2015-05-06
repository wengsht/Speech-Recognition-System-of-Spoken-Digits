function [y] = vec_file_read(filename)
    fID = fopen(filename);
    if fID < 0
        error ('open ')
    end

    y = fscanf(fID, '%f', [1 inf]);

    y = y';

