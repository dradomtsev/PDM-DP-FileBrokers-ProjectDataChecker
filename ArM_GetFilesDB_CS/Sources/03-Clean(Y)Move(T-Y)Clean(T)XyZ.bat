REM ---------------------------------------------------- Output

@ echo off
echo Script will clean Y:, then will move folders and files from T: to Y: that older than specified date 
set /p date="Input date to clean from (ex.20180128): " "
cls
echo Date to clean from = %date%
echo __________________________________________________

pause

REM ---------------------------------------------------- Clean Y:
@ echo off

rmdir "h:\deleted\XyZ\" 	/s /q

REM move files with creation time older than %date% ex.20180128 and modification time not newer than %date%
REM delete files from sources and clean empty folders in sources
REM ---------------------------------------------------- T: ---> Y:
robocopy "h:\temporary\XyZ" 	"h:\deleted\XyZ" 	/xf"h:\temporary\XyZ\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5

REM ---------------------------------------------------- end

pause
exit

REM Use it if ROBOCOPY Move-Delete does not work
REM Clean T: for folders without files
REM robocopy "h:\temporary\!TmP" "h:\temporary\!TmP" /s /move
REM robocopy "h:\temporary\ArT" "h:\temporary\ArT" /s /move
REM robocopy "h:\temporary\BrB" "h:\temporary\BrB" /s /move
REM robocopy "h:\temporary\BzH" "h:\temporary\BzH" /s /move
REM robocopy "h:\temporary\DgT" "h:\temporary\DgT" /s /move
REM robocopy "h:\temporary\DLV" "h:\temporary\DLV" /s /move
REM robocopy "h:\temporary\FbO" "h:\temporary\FbO" /s /move
REM robocopy "h:\temporary\GoL" "h:\temporary\GoL" /s /move
REM robocopy "h:\temporary\IvK" "h:\temporary\IvK" /s /move
REM robocopy "h:\temporary\KrR" "h:\temporary\KrR" /s /move
REM robocopy "h:\temporary\LpK" "h:\temporary\LpK" /s /move
REM robocopy "h:\temporary\PcH" "h:\temporary\PcH" /s /move
REM robocopy "h:\temporary\RbO" "h:\temporary\RbO" /s /move
REM robocopy "h:\temporary\RcH" "h:\temporary\RcH" /s /move
REM robocopy "h:\temporary\ScN" "h:\temporary\ScN" /s /move
REM robocopy "h:\temporary\ShE" "h:\temporary\ShE" /s /move
REM robocopy "h:\temporary\StR" "h:\temporary\StR" /s /move
REM robocopy "h:\temporary\TmK" "h:\temporary\TmK" /s /move
REM robocopy "h:\temporary\TpL" "h:\temporary\TpL" /s /move
REM robocopy "h:\temporary\UnC" "h:\temporary\UnC" /s /move
REM robocopy "h:\temporary\VdB" "h:\temporary\VdB" /s /move
REM robocopy "h:\temporary\VlK" "h:\temporary\VlK" /s /move
REM robocopy "h:\temporary\VsS" "h:\temporary\VsS" /s /move
REM robocopy "h:\temporary\ZaB" "h:\temporary\ZaB" /s /move
REM robocopy "h:\temporary\ZbL" "h:\temporary\ZbL" /s /move
REM robocopy "h:\temporary\ZrC" "h:\temporary\ZrC" /s /move
