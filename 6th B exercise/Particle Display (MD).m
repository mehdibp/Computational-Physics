%******************************************************************************%
%***                                                                       %**%
%***  Molecular dynamics viewer v0.50; Date: 1399/03/18                    %**%
%***  Copyleft 2020 by M. D. Niry. All lefts reserved.                     %**%
%***  Load and show the path.txt                                           %**%
%***                                                                       %**%
%***  The following code is developed in the Octave and may be compatible  %**%
%***  with MATLAB.                                                         %**%
%***                                                                       %**%
%******************************************************************************%

clc;
close all;
clear;

FN  = "path.txt";            
FPS = 30;                    

fid = fopen (FN, "r");

D = fscanf (fid, "%i %i", [1, 2]);
L = D(1,1);
N = D(1,2);

fig = figure(1);

while ~feof(fid) 
  for i = 1:N
    D = fscanf (fid, "%f %i", [1, 2]);
    if feof(fid)
      continue
    end
    
    t = D(1, 1);
    fprintf("t = %.2f      \r", t);
    
    D = fscanf (fid, "%f %f", [2, N]);
    x = D(1, :);
    y = D(2, :);
    plot(x, y, "ro");
    axis([0 L 0 L]);
    title([sprintf("MD Simulation box (N = %i ; t = %.2f)", N, t)]);
    
    pause(1 / FPS);
  end
  end

disp("\n\nFinish!");

fclose (fid);