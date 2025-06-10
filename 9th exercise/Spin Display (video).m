% You can use video.m to show snapshot.txt as a video in Octave.
%
% Note: Octave is a software package, which is similar to MatLab.
% Therefore video.m may be worked in the MatLab with minor changes.
% Copyleft 2016 by M. D. Niry, All lefts reserved!

fid = fopen("snapshot.txt");     % open snapshot.txt file; fid is a file handle
L = fscanf(fid, "%u", 1);         % find L in 1st line of fid
 writerObj = VideoWriter('myVideo.avi');
 writerObj.FrameRate = 1;
 open(writerObj);
 
while (~feof(fid))
  img = ones(L, L);               % generate L*L matrix, which all cells are filled with one
  for i = 1:L
    r = fscanf(fid, "%u", L);     % read one row of the image from fid
    img(i,:) = r;                 % copy r to the i'th image row
  end
  imshow(img, 'InitialMagnification', 200);                    % show a frame
  writeVideo(writerObj,img);
end
 close(writerObj);
fclose(fid);                      % close fid

pause();                          % wait
