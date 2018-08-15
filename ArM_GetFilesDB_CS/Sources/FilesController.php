<?php

namespace App\Http\Controllers;

use Debugbar;
use App\Models\Roles;
use App\Models\Files;
use App\Models\Folders;
use App\Models\Projects;
use App\Models\Companies;
use App\Models\Subsystems;
use App\Models\DataTypes;
use Illuminate\Http\Request;
use App\Http\Helpers\FolderHelper;
use App\Http\Helpers\StringHelper;
use App\Http\Helpers\FileHelper;
use Illuminate\Support\Facades\Auth;

class FilesController extends Controller
{
    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function index(Request $request)
    {
        Debugbar::startMeasure('FilesController@index');

        $files = Files::where('user_id', /*Auth::user()->id*/4)->get();
        $folders = Folders::all();
        $projects = Projects::all();

        //dd($files[0]);
        $folder = '';
        $project = '';
        $files->each(function(Files $file) use ($folders, $projects) {
            $file->errors = [];
            $folder = $folders->find($file->folder_id);

            if (FileHelper::checkMask($file) == 0) {
                $file->errors = array_merge($file->errors, [__('files.the-filename-does-not-match-the-mask')]);
                return true;
            }

            if (FileHelper::hasCyrillic($file) !== 0)
                $file->errors = array_merge($file->errors, [__('files.cyrillic-symbols-are-present-in-the-file-name')]);

            if (Projects::where('identifier', 'LIKE', FolderHelper::getProject($folder) . '%')->get()->isEmpty())
                $file->errors = array_merge($file->errors, [__('files.the-project-does-not-exist-in-the-project-database')]);

            if (FileHelper::getProject($file) !== FolderHelper::getProject($folder))
                $file->errors = array_merge($file->errors, [__('files.the-file-name-does-not-match-the-project-path')]);

            if (Companies::where('code', FileHelper::getCompany($file))->get()->isEmpty())
                $file->errors = array_merge($file->errors, [__('files.the-organization-does-not-exist-in-the-database-of-organizations')]);

            if (Roles::where('code', FileHelper::getRole($file))->get()->isEmpty()) {
                $file->errors = array_merge($file->errors, [__('files.the-role-does-not-exist-in-the-database-of-roles')]);
                $file->name = StringHelper::highlight_keyword($file->name, FileHelper::getRole($file));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (FileHelper::getStage($file) !== FolderHelper::getStage($folder)) {
                $file->errors = array_merge($file->errors, [__('files.the-stage-does-not-correspond-to-the-path')]);
                $file->name = StringHelper::highlight_keyword($file->name, FileHelper::getStage($file));
            }
            if (Projects::where('identifier', FolderHelper::getProject($folder) . '-' . FolderHelper::getStage($folder))->get()->isEmpty())
                $file->errors = array_merge($file->errors, [__('files.the-project-with-this-stage-does-not-exist-in-the-project-database')]);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (Subsystems::where('code', FileHelper::getSubsystem($file))->get()->isEmpty()) {
                $file->errors = array_merge($file->errors, [__('files.the-subsystem-does-not-exist-in-the-database-of-subsystems')]);
                $file->name = StringHelper::highlight_keyword($file->name, FileHelper::getSubsystem($file));
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (DataTypes::where('code', FileHelper::getDataType($file))->get()->isEmpty()) {
                $file->errors = array_merge($file->errors, [__('files.the-datatype-does-not-exist-in-the-database-of-datatypes')]);
                $file->name = StringHelper::highlight_keyword($file->name, FileHelper::getDataType($file));
            }

        });

        Debugbar::stopMeasure('FilesController@index');
        return view('files/index', compact('files', 'folders'));
    }
}
