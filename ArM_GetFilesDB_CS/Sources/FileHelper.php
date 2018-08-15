<?php

namespace App\Http\Helpers;

use Cache;
use Request;
use Debugbar;
use Carbon\Carbon;
use App\Models\Files;

class FileHelper {
	public static function checkMask(Files $file) {
		return preg_match('/\w+\-[\[\(]\w+[\]\)]\w+-[\[\(]\w+[\]\)][\w%]*-\w+-\w+.+/', $file->name);
	}
	public static function hasCyrillic(Files $file) {
		return preg_match('/[А-Яа-яЁёїЇіІ]/u', $file->name);
	}
	public static function getProject(Files $file) {
		return explode('-', $file->name)[0];
	}
	public static function getCompany(Files $file) {
		return preg_replace('/[\[\(](\w+)[\]\)]\w+/', '$1', explode('-', $file->name)[1]);
	}
	public static function getRole(Files $file) {
		return preg_replace('/[\[\(]\w+[\]\)](\w+)/', '$1', explode('-', $file->name)[1]);
	}
	public static function getStage(Files $file) {
		return preg_replace('/[\[\(](\w+)[\]\)].*/', '$1', explode('-', $file->name)[2]);
	}
	public static function getSubsystem(Files $file) {
		return explode('-', $file->name)[3];
	}
	public static function getDataType(Files $file) {
		return explode('.', explode('-', $file->name)[4])[0];
	}
}