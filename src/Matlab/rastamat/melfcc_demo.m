function [cepstra,aspectrum,pspectrum] = melfcc_demo(filename, varargin)
%[cepstra,aspectrum,pspectrum] = melfcc(samples, sr[, opts ...])
%  Calculate Mel-frequency cepstral coefficients by:
%   - take the absolute value of the STFT
%   - warp to a Mel frequency scale
%   - take the DCT of the log-Mel-spectrum
%   - return the first <ncep> components
%  This version allows a lot of options to be controlled, as optional 
%  'name', value pairs from the 3rd argument on: (defaults in parens)
%    'wintime' (0.025): window length in sec
%    'hoptime' (0.010): step between successive windows in sec
%    'numcep'     (13): number of cepstra to return
%    'lifterexp' (0.6): exponent for liftering; 0 = none; < 0 = HTK sin lifter
%    'sumpower'    (1): 1 = sum abs(fft)^2; 0 = sum abs(fft)
%    'preemph'  (0.97): apply pre-emphasis filter [1 -preemph] (0 = none)
%    'dither'      (0): 1 = add offset to spectrum as if dither noise
%    'minfreq'     (0): lowest band edge of mel filters (Hz)
%    'maxfreq'  (4000): highest band edge of mel filters (Hz)
%    'nbands'     (40): number of warped spectral bands to use
%    'bwidth'    (1.0): width of aud spec filters relative to default
%    'dcttype'     (2): type of DCT used - 1 or 2 (or 3 for HTK or 4 for feac)
%    'fbtype'  ('mel'): frequency warp: 'mel','bark','htkmel','fcmel'
%    'usecmp'      (0): apply equal-loudness weighting and cube-root compr.
%    'modelorder'  (0): if > 0, fit a PLP model of this order
%    'broaden'     (0): flag to retain the (useless?) first and last bands
%    'useenergy'   (0): overwrite C0 with true log energy
% The following non-default values nearly duplicate Malcolm Slaney's mfcc
% (i.e. melfcc(d,16000,opts...) =~= log(10)*2*mfcc(d*(2^17),16000) )
%       'wintime': 0.016
%     'lifterexp': 0
%       'minfreq': 133.33
%       'maxfreq': 6855.6
%      'sumpower': 0
% The following non-default values nearly duplicate HTK's MFCC
% (i.e. melfcc(d,16000,opts...) =~= 2*htkmelfcc(:,[13,[1:12]])'
%  where HTK config has PREEMCOEF = 0.97, NUMCHANS = 20, CEPLIFTER = 22, 
%  NUMCEPS = 12, WINDOWSIZE = 250000.0, USEHAMMING = T, TARGETKIND = MFCC_0)
%     'lifterexp': -22
%        'nbands': 20
%       'maxfreq': 8000
%      'sumpower': 0
%        'fbtype': 'htkmel'
%       'dcttype': 3
% For more detail on reproducing other programs' outputs, see
% http://www.ee.columbia.edu/~dpwe/resources/matlab/rastamat/mfccs.html
%
% 2005-04-19 dpwe@ee.columbia.edu after rastaplp.m.  
% Uses Mark Paskin's process_options.m from KPMtools
% $Header: /Users/dpwe/matlab/rastamat/RCS/melfcc.m,v 1.3 2012/09/03 14:01:26 dpwe Exp dpwe $

if nargin < 2;   sr = 44100;    end

splite = 2;
[samples, sr] = wavread(filename);
% Parse out the optional arguments
[wintime, hoptime, numcep, lifterexp, sumpower, preemph, dither, ...
 minfreq, maxfreq, nbands, bwidth, dcttype, fbtype, usecmp, modelorder, ...
 broaden, useenergy] = ...
    process_options(varargin, 'wintime', 0.025, 'hoptime', 0.010, ...
          'numcep', 13, 'lifterexp', 0.6, 'sumpower', 1, 'preemph', 0.97, ...
	  'dither', 0, 'minfreq', 0, 'maxfreq', 4000, ...
	  'nbands', 40, 'bwidth', 1.0, 'dcttype', 2, ...
	  'fbtype', 'htkmel', 'usecmp', 0, 'modelorder', 0, ...
          'broaden', 0, 'useenergy', 0);

figure(1);
plot(samples)

if preemph ~= 0
  samples = filter([1 -preemph], 1, samples); 
end


%%%
figure(2);
subplot(splite, 1, 1);
plot(samples * 32768);
title('\fontsize{15}PreEmphasize: rastamat');
subplot(splite, 1, 2);
y = load('emp.txt');
plot(y);
title('\fontsize{15}PreEmphasize: C Plus Plus');
%%%

% Compute FFT power spectrum
[pspectrum,logE] = powspec(samples, sr, wintime, hoptime, dither);

%%%
figure(3);
subplot(splite, 1, 1);
plot(pspectrum);
title('\fontsize{15}Pow Spectrum: rastamat');
subplot(splite, 1, 2);
y = load('powSpec.txt')';
plot(y);
title('\fontsize{15}Pow Spectrum: C Plus Plus');
%%%

aspectrum = audspec(pspectrum, sr, nbands, fbtype, minfreq, maxfreq, sumpower, bwidth);

if (usecmp)
  % PLP-like weighting/compression
  aspectrum = postaud(aspectrum, maxfreq, fbtype, broaden);
end

%%%
figure(4);
subplot(splite, 1, 1);
pcolor(log(aspectrum));
colormap(jet(10000))
shading flat
title('\fontsize{15}Mel Log Spectrum: rastamat');
subplot(splite, 1, 2);
y = load('melLogSpec.txt');
pcolor(y);
colormap(jet(10000))
shading flat
title('\fontsize{15}Mel Log Spectrum: C Plus Plus');
%%%

if modelorder > 0

  if (dcttype ~= 1) 
    disp(['warning: plp cepstra are implicitly dcttype 1 (not ', num2str(dcttype), ')']);
  end
  
  % LPC analysis 
  lpcas = dolpc(aspectrum, modelorder);

  % convert lpc to cepstra
  cepstra = lpc2cep(lpcas, numcep);

  % Return the auditory spectrum corresponding to the cepstra?
%  aspectrum = lpc2spec(lpcas, nbands);
  % else return the aspectrum that the cepstra are based on, prior to PLP

else
  % Convert to cepstra via DCT
  cepstra = spec2cep(aspectrum, numcep, dcttype);

end

%%%
figure(5);
subplot(splite, 1, 1);
pcolor(cepstra);
shading flat
title('\fontsize{15}Mel Cepstrum: rastamat');
subplot(splite, 1, 2);
y = load('melCeps.txt')';
pcolor(y);
colormap(jet(10000));
shading flat

title('\fontsize{15}Mel Cepstrum: C Plus Plus');
%%%

cepstra = lifter(cepstra, lifterexp);

%%%
figure(6);
subplot(1+splite, 1, 1);
y = load('melCeps.txt')';
plot(y);
subplot(1+splite, 1, 2);
y = load('normalMelCeps.txt')';
plot(y);
title('\fontsize{15}Normalized Mel Cepstrum: C Plus Plus');
subplot(1+splite, 1, 3);
pcolor(y);
colormap(jet(10000))
shading flat
title('\fontsize{15}Normalized Mel Cepstrum: C Plus Plus');
%%%

if useenergy
  cepstra(1,:) = logE;
end
  

