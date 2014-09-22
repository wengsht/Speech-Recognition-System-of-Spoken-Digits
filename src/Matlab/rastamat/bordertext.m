% Usage:
%       htext = bordertext( 'Position', 'YourText');
%       htext = bordertext( 'Position', 'YourText', '[FormatString]' );
%       htext = bordertext( 'Position', 'YourText', 'Prop1', Value1, 'Prop2', Value2, ... );
%
%       run 'bordertext' without arguments to see an example.
%   
% Position:
%                'topleft'           'top'          'topright'
%       'lefttop'|-------------------------------------------|'righttop'
%                |                                           |
%          'left'|                  'center'                 |'right'
%                |                                           |
%    'leftbottom'|-------------------------------------------|'rightbottom'
%                'bottomleft'       'bottom'     'bottomright'
% 
%       This 'Position'-string can be prefixed with 'inner' or 'figure'.
%       e.g: 'innertopleft' or 'figurebottomright'
%
%       'Position' can also be an [ X Y ]-position wrt the current axis.
%
% Text:
%   Your own string:    'Hello world'
%   or with more lines: 'Hello World\nNice weather\nisn''t it?'
%   or with more lines: {'Hello World','Nice weather','isn''t it?'}
%
% FormatString:
%   FontSize:           [%f] (any positive floatingpoint value)
%   TextZoom:           [ Z] Text zooms when zooming your plot (only when'Position' is an [ X Y ]-position)
%   FontAttributes:      [B]  [I]  [L]  [T] (Bold, Italic, Latex, Tex)
%   FontColors:          [w]  [k]  [r]  [g]  [b]  [c]  [m]  [y]
%   FontColors:         [fw] [fk] [fr] [fg] [fb] [fc] [fm] [fy]
%   BackgroundColors:   [bw] [bk] [br] [bg] [bb] [bc] [bm] [by]
%   EdgeColors:         [ew] [ek] [er] [eg] [eb] [ec] [em] [ey]
%   TextRotation:       [R%f] (any floatingpoint value)    
%   HorizontalAlignment:[Hl] [Hc] [Hr]  (normally you don't need these)
%   VerticalAlignment:  [Vb] [Vm] [Vt]  (normally you don't need these)
%
%   Examples:
%     bordertext( 'innertopleft', '1^r^s^t line\n2^n^d line', '[B][I][T][fr][bg][ek][14][R10]' );
%     bordertext( 'innertopleft', '1^r^s^t line\n2^n^d line', 'B I T fr bg ek 14 R10' );
%     bordertext( [0.5 0.5], 'TextZoom', 'TextZoom', 'on' );
%
% Example with matlab syntax:
%   bordertext( 'innertopleft', {'1^r^s^t line','2^n^d line'}, ...
%     'FontWeight', 'Bold', 'FontAngle', 'Italic', 'Interpret', 'Tex', ...
%     'Color', 'r', 'BackgroundColor', 'g', 'EdgeColor', [0 0 0], ...
%     'FontSize', 14, 'Rotation', 10 );
%      
% To remove a bordertext-string from the figure:
%   bordertext( <POSITION> , '' );   example: bordertext( 'innertopleft', '' );
%
% Last release:
% V 2.08  27-may-2014  Option to 'zoom' your text (only when'Position' is an [ X Y ]-position)
%                      Ex: bordertext( [0.5 0.5], 'TextZoom', 'TextZoom', 'on' );   zoom on;
%
% Author: Jan G. de Wilde   email: jan.dewilde@nl.thalesgroup.com
%         Thales Nederland B.V.

% V 1.00  22-sep-2006  First release.
% V 2.00  01-apr-2013  Text stays on axis, also while zooming, panning or resizing.
% V 2.01  04-apr-2013  Fix for reverse X- and Y-axes.
% V 2.02  05-apr-2013  Some bugfixes.
% V 2.03  10-apr-2013  More flexible in interpreting 'FormatString'.
% V 2.04  12-apr-2013  Now able to handle linked-axes.
% V 2.05  16-apr-2013  Make '[ X Y ]'-text invisible outside axis.
% V 2.06  10-sep-2013  Support matlab syntax: bordertext( 'top', 'hello', 'Color', [1 0 0], 'FontSize', 20 );
%                      text.Margin = eps; coloured background looks nicer.
% V 2.07  25-feb-2014  Minor changes.
% V 2.08  27-may-2014  Option to 'zoom' your text (only when'Position' is an [ X Y ]-position)
%                      Use 'Clipping' to 'on' for 'userdefined'-text.

function h = bordertext( varargin )

VERSION = GetVersion( mfilename('fullpath') );  % V 2.07

if nargin == 0, % Help, Test and Debug mode
    help bordertext
    
    figure(999);	clf('reset');
    plot( randn(1,100), '+', 'MarkerSize', 4 );	axis([ 0 100 -3 3 ]);   pan on;
%     set(gca,'XDir','reverse');   set(gca,'YDir','reverse');
%     imagesc(rand(25),[-1 1]); colormap(gray); axis auto tight square
    title('title');   xlabel('xlabel');   ylabel('ylabel');

    bordertext('topleft','topleft');
    bordertext('top','top');
    bordertext('topright','topright');
    bordertext('righttop','righttop');
    bordertext('right','right');
    bordertext('rightbottom','rightbottom');
    bordertext('bottomright','bottomright');
    bordertext('bottom','bottom');
    bordertext('bottomleft','bottomleft');
    bordertext('leftbottom','leftbottom');
    bordertext('left','left');
    bordertext('lefttop','lefttop');
    bordertext('center','center');
    
    bordertext('innertopleft','innertopleft');
    bordertext('innertop','innertop');
    bordertext('innertopright','innertopright');
    bordertext('innerrighttop','innerrighttop');
    bordertext('innerright','innerright');
    bordertext('innerrightbottom','innerrightbottom');
    bordertext('innerbottomright','innerbottomright');
    bordertext('innerbottom','innerbottom');
    bordertext('innerbottomleft','innerbottomleft');
    bordertext('innerleftbottom','innerleftbottom');
    bordertext('innerleft','innerleft');
    bordertext('innerlefttop','innerlefttop');
    
    bordertext('figuretopleft','figuretopleft');
    bordertext('figuretop','figuretop');
    bordertext('figuretopright','figuretopright');
    bordertext('figurerighttop','figurerighttop');
    bordertext('figureright','figureright');
    bordertext('figurerightbottom','figurerightbottom');
    bordertext('figurebottomright','figurebottomright');
    bordertext('figurebottom','figurebottom');
    bordertext('figurebottomleft','figurebottomleft');
    bordertext('figureleftbottom','figureleftbottom');
    bordertext('figureleft','figureleft');
    bordertext('figurelefttop','figurelefttop');
    bordertext('figurecenter','figurecenter');
    
    bordertext([50  2.0],'h = bordertext( ''Position'', ''Text'' [,''FormatString'' ]);','[r][13][Z][Hc]');
    bordertext([50  1.5],'h = bordertext( ''Position'', ''Text'' [,''Property1'', Value1, ... ]);','[r][12][Z][Hc]');
    bordertext([50 -2.0], VERSION,'[m][13][Hc][Z]');

    return
end

ax      = axis;
curr_ax = gca;
fnr     = gcf;
Xrev    = strcmp( get( curr_ax, 'XDir' ), 'reverse' );   % V 2.01
Yrev    = strcmp( get( curr_ax, 'YDir' ), 'reverse' );

% Parse inputs
if nargin >= 1, pos         = varargin{1};  end
if nargin >= 2, txt         = varargin{2};  end
if nargin == 3, FormatStr   = varargin{3};  end
if nargin >= 4, 
    for ind = 3 : 2 : length( varargin ) - 1,
        FormatStr.(varargin{ind}) = varargin{ind+1};
    end
end

% This is the CallBack-function while zooming, panning or resizing
if nargin == 1, 
    if strcmp( pos, 'callback' ),    
        % from GetLinkedAxes() ...
        tmp = getappdata( curr_ax );    % V 2.04
        if isfield( tmp, 'graphics_linkaxes' ),
            Targets = get( tmp.graphics_linkaxes, 'Targets' );
            
            linked_axes = NaN( 1, length( Targets ) );
            for iTargets = 1 : length( Targets ),
                tmpChildren             = get( Targets( iTargets ), 'Children' );
                linked_axes( iTargets ) = get( tmpChildren(1), 'Parent' );
            end
        else
            linked_axes = curr_ax;
        end

        for curr_ax = linked_axes,    % V 2.04
            UserData    = get( curr_ax, 'UserData' );
            if ~isfield( UserData, 'BordertextHandles' ),   return;   end
            
            bt_handles  = UserData.BordertextHandles;
            if isfield( bt_handles, 'userdefined' ),    % V 2.08
                ax = axis;   dx = abs( ax(2) - ax(1) );   dy = abs( ax(4) - ax(3) );
                for ih = 1 : length( UserData.BordertextHandles.userdefined ),
                    htxt = UserData.BordertextHandles.userdefined( ih );
                    zmXY = UserData.BordertextHandles.userdefined_textzoom( ih );
                    if ishandle( htxt ) && zmXY,
                        zmX = UserData.BordertextHandles.userdefined_dx( ih ) / dx;
                        zmY = UserData.BordertextHandles.userdefined_dy( ih ) / dy;
                        set( htxt, 'FontSize', min( zmX, zmY ) * abs( UserData.BordertextHandles.userdefined_fsize( ih ) ) );
                    end
                end
            end
            
            AllFields = fieldnames( bt_handles );
            for iflds = 1 : length( AllFields ),
                FieldName = AllFields( iflds ); FieldName = FieldName{1};
                th = UserData.BordertextHandles.( FieldName );
                if ~ishandle( th ),   continue;   end
                
                if strncmpi( FieldName, 'inner', 5 ),
                    FieldName = FieldName( 6 : end );
                end
                
                Xnew = NaN;   Ynew = NaN;   Znew = 0;
                switch FieldName,
                    case 'topleft',     Xnew = ax(1+Xrev);          Ynew = ax(4-Yrev);
                    case 'top',         Xnew = (ax(1)+ax(2))/2;     Ynew = ax(4-Yrev);
                    case 'topright',    Xnew = ax(2-Xrev);          Ynew = ax(4-Yrev);
                    case 'righttop',    Xnew = ax(2-Xrev);          Ynew = ax(4-Yrev);
                    case 'right',       Xnew = ax(2-Xrev);          Ynew = (ax(3)+ax(4))/2;
                    case 'rightbottom', Xnew = ax(2-Xrev);          Ynew = ax(3+Yrev);
                    case 'bottomright', Xnew = ax(2-Xrev);          Ynew = ax(3+Yrev);
                    case 'bottom',      Xnew = (ax(1)+ax(2))/2;     Ynew = ax(3+Yrev);
                    case 'bottomleft',  Xnew = ax(1+Xrev);          Ynew = ax(3+Yrev);
                    case 'leftbottom',  Xnew = ax(1+Xrev);          Ynew = ax(3+Yrev);
                    case 'left',        Xnew = ax(1+Xrev);          Ynew = (ax(3)+ax(4))/2;
                    case 'lefttop',     Xnew = ax(1+Xrev);          Ynew = ax(4-Yrev);
                    case 'center',      Xnew = (ax(1)+ax(2))/2;     Ynew = (ax(3)+ax(4))/2;
                    otherwise,          % skip
                end
                if ~isnan( Xnew ),   set( th, 'Position', [ Xnew Ynew Znew ] );   end
            end
        end
    else
        fprintf('Usage: htext = bordertext( ''Position'', ''YourText'', ''[FormatString]'' );\n');
        fprintf('Usage: htext = bordertext( ''Position'', ''YourText'', FormatStruct );\n');
        fprintf('Usage: htext = bordertext( ''Position'', ''YourText'', ''Propery1'', Value1, ... );\n');
        error('Too few arguments.');
    end
    
    return
end

if nargin == 2,   FormatStr = [];   end

% Make once in each figure an invisisible axes for text on 'figure'-level.
UserData = get( fnr, 'UserData' );
if ~isfield( UserData, 'BordertextHandles' ) || ~ishandle( UserData.BordertextHandles ),
    UserData.BordertextHandles.h_axes = axes( 'Unit', 'Normalized', ...
        'Position', [0 0 1 1], 'Visible', 'off', 'Nextplot', 'add', 'Hittest','off');
    set( fnr, 'UserData', UserData, 'CurrentAxes', curr_ax );
end
h_axes = UserData.BordertextHandles.h_axes;

if ~ischar( pos ),
   UserPos = pos;   pos = 'userdefined';
end

%% Interpret/decode the FormatStruct / FormatString
if ~isempty( FormatStr )  &&  ~isstruct( FormatStr ),
    FormatString = FormatStr;   FormatStr = [];   % V 2.06
    FormatString( FormatString == '[' ) = ' ';
    FormatString( FormatString == ']' ) = ' ';
    
    SpacePos = find( FormatString == ' ' );
    SpacePos = [ 0 SpacePos length(FormatString)+1 ];
    
    for i = 1 : length( SpacePos ) - 1,        
        cmd = FormatString( SpacePos(i)+1 : SpacePos(i+1)-1 ) ;
        cmd( isspace( cmd ) ) = '';
        if isempty( cmd ), 	 continue;   end

        switch cmd,            
            case 'B',   FormatStr.FontWeight    = 'Bold';   % Bold
            case 'I',   FormatStr.FontAngle     = 'Italic'; % Italic
            case 'L',   FormatStr.Interpreter   = 'Latex';  % Latex interpreter
            case 'T',   FormatStr.Interpreter   = 'Tex';    % Tex interpreter
                
            case {'w','k','r','g','b','c','m','y'},         % FontColors
                FormatStr.Color = cmd;

            case {'fw','fk','fr','fg','fb','fc','fm','fy'},	% FontColors
                FormatStr.Color = cmd(2);
                
            case {'bw','bk','br','bg','bb','bc','bm','by'},	% BackgroundColors
                FormatStr.BackgroundColor = cmd(2);   
                
            case {'ew','ek','er','eg','eb','ec','em','ey'}, % EdgeColors
                FormatStr.EdgeColor = cmd(2);     
                
            case 'Hl',	FormatStr.HorizontalAlignment = 'left';       % Alignments
            case 'Hc',  FormatStr.HorizontalAlignment = 'center';
            case 'Hr',  FormatStr.HorizontalAlignment = 'right';
                
            case 'Vb',  FormatStr.VerticalAlignment = 'bottom';
            case 'Vm',  FormatStr.VerticalAlignment = 'middle';
            case 'Vt',  FormatStr.VerticalAlignment = 'top';
              
            case 'Z',   FormatStr.TextZoom          = 'on';  	% Zoomable Text
                
            otherwise,
                if cmd(1) == 'R',               % Rotation
                    FormatStr.Rotation   = str2double( cmd( 2 : end ) );
                    continue;
                end
                
                tmp = abs( str2double( cmd ) ); % FontSize
                if isnan( tmp ),
                    fprintf('%s :: Unrecognized substring: ''%s''\n', ...
                        mfilename, cmd );
                else
                    FormatStr.FontSize = tmp;
                end
        end
    end
end

% Coordinates in HiddenAxis
if strncmpi( pos, 'figure', 6 ),
    set( fnr, 'CurrentAxes', h_axes );
    xpos_left = 0.0;    xpos_middle = 0.5;    xpos_right  = 1.0;
    ypos_top  = 1.0;    ypos_middle = 0.5;    ypos_bottom = 0.0;
end

% Handle multi-line
if ~iscell( txt ),
    ind = strfind( txt, '\n' );
    if isempty( ind ),
        txt = [ ' ' txt ' ' ];
    else
        str = [];
        ind = [ -1 ind length(txt)+1 ];
        for i = 2 : length( ind ),
            str{i-1} = txt(ind(i-1)+2:ind(i)-1); %#ok<AGROW>
        end
        txt = str;   clear str;
    end
end

% Translate 'pos' to [X Y]-coordinates
switch lower( pos ),
    case 'topleft',
        h = text( ax(1+Xrev), ax(4-Yrev), txt );
        set( h, 'Horizontal', 'left', 'Vertical', 'bottom', 'Rotation', 0 );
        
    case 'top',
        h = text( (ax(1)+ax(2))/2, ax(4-Yrev), txt );
        set( h, 'Horizontal', 'center', 'Vertical', 'bottom', 'Rotation', 0 );
        
    case 'topright',
        h = text( ax(2-Xrev), ax(4-Yrev), txt );
        set( h, 'Horizontal', 'right', 'Vertical', 'bottom', 'Rotation', 0 );
        
    case 'righttop',
        h = text( ax(2-Xrev), ax(4-Yrev), txt );
        set( h, 'Horizontal', 'right', 'Vertical', 'top', 'Rotation', 90 );
        
    case 'right',
        h = text( ax(2-Xrev), (ax(3)+ax(4))/2, txt );
        set( h, 'Horizontal', 'center', 'Vertical', 'top', 'Rotation', 90 );
        
    case 'rightbottom',
        h = text( ax(2-Xrev), ax(3+Yrev), txt );
        set( h, 'Horizontal', 'left', 'Vertical', 'top', 'Rotation', 90 );
        
    case 'bottomright',
        h = text( ax(2-Xrev), ax(3+Yrev), txt );
        set( h, 'Horizontal', 'right', 'Vertical', 'top', 'Rotation', 0 );
        
    case 'bottom',
        h	= text( (ax(1)+ax(2))/2, ax(3+Yrev), txt );
        set( h, 'Horizontal', 'center', 'Vertical', 'top', 'Rotation', 0 );
        
    case 'bottomleft',
        h = text( ax(1+Xrev), ax(3+Yrev), txt );
        set( h, 'Horizontal', 'left', 'Vertical', 'top', 'Rotation', 0 );
        
    case 'leftbottom',
        h = text( ax(1+Xrev), ax(3+Yrev), txt );
        set( h, 'Horizontal', 'left', 'Vertical', 'bottom', 'Rotation', 90 );
        
    case 'left',
        h = text( ax(1+Xrev), (ax(3)+ax(4))/2, txt );
        set( h, 'Horizontal', 'center', 'Vertical', 'bottom', 'Rotation', 90 );
        
    case 'lefttop',
        h = text( ax(1+Xrev), ax(4-Yrev), txt );
        set( h, 'Horizontal', 'right', 'Vertical', 'bottom', 'Rotation', 90 );
        
    case 'center',
        h = text( (ax(1)+ax(2))/2, (ax(3)+ax(4))/2, txt );
        set( h, 'Horizontal', 'center', 'Vertical', 'middle', 'Rotation', 0 );
        
    case 'innertopleft',
        h = text( ax(1+Xrev), ax(4-Yrev), txt );
        set( h, 'Horizontal', 'left', 'Vertical', 'top', 'Rotation', 0 );
        
    case 'innertop',
        h = text( (ax(1)+ax(2))/2, ax(4-Yrev), txt );
        set( h, 'Horizontal', 'center', 'Vertical', 'top', 'Rotation', 0 );
        
    case 'innertopright',
        h = text( ax(2-Xrev), ax(4-Yrev), txt );
        set( h, 'Horizontal', 'right', 'Vertical', 'top', 'Rotation', 0 );
        
    case 'innerrighttop',
        h = text( ax(2-Xrev), ax(4-Yrev), txt );
        set( h, 'Horizontal', 'right', 'Vertical', 'bottom', 'Rotation', 90 );
        
    case 'innerright',
        h = text( ax(2-Xrev), (ax(3)+ax(4))/2, txt );
        set( h, 'Horizontal', 'center', 'Vertical', 'bottom', 'Rotation', 90 );
        
    case 'innerrightbottom',
        h = text( ax(2-Xrev), ax(3+Yrev), txt );
        set( h, 'Horizontal', 'left', 'Vertical', 'bottom', 'Rotation', 90 );
        
    case 'innerbottomright',
        h = text( ax(2-Xrev), ax(3+Yrev), txt );
        set( h, 'Horizontal', 'right', 'Vertical', 'bottom', 'Rotation', 0 );
        
    case 'innerbottom',
        h = text( (ax(1)+ax(2))/2, ax(3+Yrev), txt );
        set( h, 'Horizontal', 'center', 'Vertical', 'bottom', 'Rotation', 0 );
        
    case 'innerbottomleft',
        h = text( ax(1+Xrev), ax(3+Yrev), txt );
        set( h, 'Horizontal', 'left', 'Vertical', 'bottom', 'Rotation', 0 );
        
    case 'innerleftbottom',
        h = text( ax(1+Xrev), ax(3+Yrev), txt );
        set( h, 'Horizontal', 'left', 'Vertical', 'top', 'Rotation', 90 );
        
    case 'innerleft',
        h = text( ax(1+Xrev), (ax(3)+ax(4))/2, txt );
        set( h, 'Horizontal', 'center', 'Vertical', 'top', 'Rotation', 90 );
        
    case 'innerlefttop',
        h = text( ax(1+Xrev), ax(4-Yrev), txt );
        set( h, 'Horizontal', 'right', 'Vertical', 'top', 'Rotation', 90 );

    case 'figuretopleft',
        h = text( xpos_left, ypos_top, txt );
        set( h, 'Horizontal', 'left', 'Vertical', 'top', 'Rotation', 0 );
        
    case 'figuretop',
        h = text( xpos_middle, ypos_top, ax(4), txt );
        set( h, 'Horizontal', 'center', 'Vertical', 'top', 'Rotation', 0 );
        
    case 'figuretopright',
        h = text( xpos_right, ypos_top, txt );
        set( h, 'Horizontal', 'right', 'Vertical', 'top', 'Rotation', 0 );
        
    case 'figurerighttop',
        h = text( xpos_right, ypos_top, [ txt ' ' ] );
        set( h, 'Horizontal', 'right', 'Vertical', 'bottom', 'Rotation', 90 );
        
    case 'figureright',
        h = text( xpos_right, ypos_middle, txt );
        set( h, 'Horizontal', 'center', 'Vertical', 'bottom', 'Rotation', 90 );
        
    case 'figurerightbottom',
        h = text( xpos_right, ypos_bottom, txt );
        set( h, 'Horizontal', 'left', 'Vertical', 'bottom', 'Rotation', 90 );
        
    case 'figurebottomright',
        h = text( xpos_right, ypos_bottom, txt );
        set( h, 'Horizontal', 'right', 'Vertical', 'bottom', 'Rotation', 0 );
        
    case 'figurebottom',
        h = text( xpos_middle, ypos_bottom, txt );
        set( h, 'Horizontal', 'center', 'Vertical', 'bottom', 'Rotation', 0 );
        
    case 'figurebottomleft',
        h = text( xpos_left, ypos_bottom, txt );
        set( h, 'Horizontal', 'left', 'Vertical', 'bottom', 'Rotation', 0 );
        
    case 'figureleftbottom',
        h = text( xpos_left, ypos_bottom, txt );
        set( h, 'Horizontal', 'left', 'Vertical', 'top', 'Rotation', 90 );
        
    case 'figureleft',
        h = text( xpos_left, ypos_middle, txt );
        set( h, 'Horizontal', 'center', 'Vertical', 'top', 'Rotation', 90 );
        
    case 'figurelefttop',
        h = text( xpos_left, ypos_top, [ txt ' ' ] );
        set( h, 'Horizontal', 'right', 'Vertical', 'top', 'Rotation', 90 );
        
    case 'figurecenter',
        h = text( xpos_middle, ypos_middle, txt );
        set( h, 'Horizontal', 'center', 'Vertical', 'middle', 'Rotation', 0 );
        
    case 'userdefined',
        h = text( UserPos(1), UserPos(2), -1e99, txt, 'Clipping', 'on' );    % V 2.08
        
    otherwise,
        fprintf('%s :: Unrecognized ''Position'': ''%s''\n', ...
                        mfilename, pos );
        clear h;   return;
end

% Set Text-Properties
if ~isfield( FormatStr, 'FontSize' ),
    if strncmpi( pos, 'figure', 6 ),    FormatStr.FontSize  = 12;
    else                                FormatStr.FontSize  =  8;
    end
end

if ~isfield( FormatStr, 'Margin' ),     FormatStr.Margin    = eps;   end

TextZoom    = 'off';
for fn = fieldnames( FormatStr )',
    fld = char(fn);
    if strcmp( fld, 'TextZoom' ),
        TextZoom = FormatStr.TextZoom;   continue;
    end
    
    set( h, fld, FormatStr.( fld ) );
end

set( fnr, 'CurrentAxes', curr_ax );     % Set scope to current axis

% Store handles in the figure of in the subplot(axis)
if strcmpi( pos, 'userdefined' ),
    UserData = get( curr_ax, 'UserData' );    % V 2.05
    if ~isfield( UserData, 'BordertextHandles' ),   UserData.BordertextHandles = struct();   end
    
    if ~isfield( UserData.BordertextHandles, 'userdefined' ), iud = 1;
    else                                                      iud = length( UserData.BordertextHandles.userdefined ) + 1;
    end
    UserData.BordertextHandles.userdefined( iud )           = h;
    UserData.BordertextHandles.userdefined_dx( iud )        = abs( ax(2) - ax(1) );
    UserData.BordertextHandles.userdefined_dy( iud )        = abs( ax(4) - ax(3) );
    UserData.BordertextHandles.userdefined_fsize( iud )     = FormatStr.FontSize;
    UserData.BordertextHandles.userdefined_textzoom( iud )  = strcmpi( TextZoom, 'on' );
    set( curr_ax, 'UserData', UserData );
else
    if strncmpi( pos, 'figure', 6 ),    h_curr = fnr;
    else                                h_curr = curr_ax;
    end
    UserData = get( h_curr, 'UserData' );
    if isfield( UserData, 'BordertextHandles' ),
        if isfield( UserData.BordertextHandles, pos ),
            if ishandle( UserData.BordertextHandles.(pos) ),
                delete( UserData.BordertextHandles.(pos) );
            end
            UserData.BordertextHandles = rmfield( UserData.BordertextHandles, pos );
        end
    end
    UserData.BordertextHandles.(pos) = h;
    set( h_curr, 'UserData', UserData );
end

% Set Callback functions for zooming and panning
set( zoom( fnr ), 'actionpostcallback',     'bordertext(''callback'')' );
set( pan(  fnr ), 'actionpostcallback',     'bordertext(''callback'')' );
set(       fnr,   'WindowButtonMotionFcn',  'bordertext(''callback'')' );
set(       fnr,   'ResizeFcn',              'bordertext(''callback'')' );

if nargout == 0;   clear h;   end

return

%% function Version = GetVersion( fn )
function Version = GetVersion( fn )
    fid = fopen([ fn '.m' ], 'r' );
    if fid > 0,
        while true,
            l = fgetl( fid );
            if l == -1,   break;   end % EOF
            
            ind = strfind( l, 'function' );
            if ~isempty( ind ),
                if ind(1) == 1,   break;   end
            end
            
            ind = strfind( l, '% V' );
            if ~isempty( ind )  &&  ( ind(1) == 1 ),
                ind = strfind( l, '-20' );
                if isempty( ind ),  Version = l( 3 : end );
                else                Version = l( 3 : ind(1)+4 );
                end
            end
        end
        fclose( fid );
    else
        Version = 'Not found.';
    end
return