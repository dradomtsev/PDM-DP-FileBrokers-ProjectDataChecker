<?php

namespace App\Http\Helpers;

use Cache;
use Request;
use Debugbar;
use Carbon\Carbon;
use App\Models\Folders;

class FolderHelper {
	public static function getProject(Folders $folder) {
		return explode('\\', $folder->path)[2];
	}
	public static function getScope(Folders $folder) {
		return explode('\\', $folder->path)[3];
	}
	public static function getStage(Folders $folder) {
		return explode('-', explode('\\', $folder->path)[4])[1];
	}
	/*public static function getQueue(Folders $folder) {
		return explode('\\', $folder->path)[5];
	}*/
	public static function getGroup(Folders $folder) {
		return explode('\\', $folder->path)[6];
	}
	public static function getRole(Folders $folder) {
		return explode('\\', $folder->path)[7];
	}
}