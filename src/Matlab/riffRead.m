function [d, fs] = riffRead(filename)
    fID = fopen(filename);

    if fID < 0
        error('open file error')
    end

    riff = fread(fID, 4, 'char');
    if(any(riff ~= ['RIFF']'))
        fclose(fID);
        error('RIFF ')
    end

    fLen = fread(fID, 1, 'int');

    waveFmt = fread(fID, 8, 'char');

    if(any(waveFmt ~= ['WAVEfmt ']'))
        fclose(fID);
        error('waveFmt ')
    end

    noUse = fread(fID, 1, 'uint');

    FormatCategory = fread(fID, 1, 'short');

    NChannels = fread(fID, 1, 'short');

    fs = fread(fID, 1, 'uint');
    
    sampleByte = fread(fID, 1, 'int');

    bytesPerSample = fread(fID, 1, 'short');

    nBitsPerSample= fread(fID, 1, 'short');

    data = fread(fID, 4, 'char');

    if(any(data ~= ['data']'))
        fclose(fID);
        error('DATA ')
    end

    rawLen = fread(fID, 1, 'int');

    if bytesPerSample == 1
        d = fread(fID, rawLen, 'char');
    elseif bytesPerSample == 2
        d = fread(fID, rawLen / 2, 'short');
    elseif bytesPerSample == 4
        d = fread(fID, rawLen / 4, 'int');
    elseif bytesPerSample == 8
        d = fread(fID, rawLen / 8, 'double');
    end;

    fclose(fID);
