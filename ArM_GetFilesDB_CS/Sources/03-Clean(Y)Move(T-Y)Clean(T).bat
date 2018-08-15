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
rmdir "h:\deleted\!TmP\" 	/s
rmdir "h:\deleted\ArT\" 	/s
rmdir "h:\deleted\BrB\" 	/s
rmdir "h:\deleted\BzH\" 	/s
rmdir "h:\deleted\DgT\" 	/s
rmdir "h:\deleted\DLV\" 	/s
rmdir "h:\deleted\FbO\" 	/s
rmdir "h:\deleted\GoL\" 	/s
rmdir "h:\deleted\IvK\" 	/s
rmdir "h:\deleted\KrR\" 	/s
rmdir "h:\deleted\LpK\" 	/s
rmdir "h:\deleted\PcH\" 	/s
rmdir "h:\deleted\RbO\" 	/s
rmdir "h:\deleted\RcH\" 	/s
rmdir "h:\deleted\ScN\" 	/s
rmdir "h:\deleted\ShE\" 	/s
rmdir "h:\deleted\StR\" 	/s
rmdir "h:\deleted\TmK\" 	/s
rmdir "h:\deleted\TpL\" 	/s
rmdir "h:\deleted\UnC\" 	/s
rmdir "h:\deleted\VdB\" 	/s
rmdir "h:\deleted\VLK\" 	/s
rmdir "h:\deleted\VsS\" 	/s
rmdir "h:\deleted\ZaB\" 	/s
rmdir "h:\deleted\ZbL\" 	/s
rmdir "h:\deleted\ZrC\" 	/s

REM move files with creation time older than %date% ex.20180128 and modification time not newer than %date%
REM delete files from sources and clean empty folders in sources
REM ---------------------------------------------------- T: ---> Y:
robocopy "h:\temporary\!TmP" 	"h:\deleted\!TmP" 	/xf"h:\temporary\!TmP\PlcHldr_NotDelbyRobocopy.txt" /zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\ArT" 	"h:\deleted\ArT" 	/xf"h:\temporary\ArT\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\BrB" 	"h:\deleted\BrB" 	/xf"h:\temporary\BrB\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\BzH" 	"h:\deleted\BzH" 	/xf"h:\temporary\BzH\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\DgT" 	"h:\deleted\DgT" 	/xf"h:\temporary\DgT\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\DLV" 	"h:\deleted\DLV" 	/xf"h:\temporary\DLV\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\FbO" 	"h:\deleted\FbO" 	/xf"h:\temporary\FbO\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\GoL" 	"h:\deleted\GoL" 	/xf"h:\temporary\GoL\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\IvK" 	"h:\deleted\IvK" 	/xf"h:\temporary\IvK\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\KrR" 	"h:\deleted\KrR" 	/xf"h:\temporary\KrR\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\LpK" 	"h:\deleted\LpK" 	/xf"h:\temporary\LpK\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\PcH" 	"h:\deleted\PcH" 	/xf"h:\temporary\PcH\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\RbO" 	"h:\deleted\RbO" 	/xf"h:\temporary\RbO\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\RcH" 	"h:\deleted\RcH" 	/xf"h:\temporary\RcH\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\ScN" 	"h:\deleted\ScN" 	/xf"h:\temporary\ScN\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\ShE" 	"h:\deleted\ShE" 	/xf"h:\temporary\ShE\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\StR" 	"h:\deleted\StR" 	/xf"h:\temporary\StR\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\TmK" 	"h:\deleted\TmK" 	/xf"h:\temporary\TmK\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\TpL" 	"h:\deleted\TpL" 	/xf"h:\temporary\TpL\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\UnC" 	"h:\deleted\UnC" 	/xf"h:\temporary\UnC\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\VdB" 	"h:\deleted\VdB" 	/xf"h:\temporary\VdB\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\VLK" 	"h:\deleted\VLK" 	/xf"h:\temporary\VLK\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\VsS" 	"h:\deleted\VsS" 	/xf"h:\temporary\VsS\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\ZaB" 	"h:\deleted\ZaB" 	/xf"h:\temporary\ZaB\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\ZbL" 	"h:\deleted\ZbL" 	/xf"h:\temporary\ZbL\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5
robocopy "h:\temporary\ZrC" 	"h:\deleted\ZrC" 	/xf"h:\temporary\ZrC\PlcHldr_NotDelbyRobocopy.txt" 	/zb /s /sl /dcopy:T /copy:DATSO /move /minage:%date% /minlad:%date% /r:2 /w:5

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
