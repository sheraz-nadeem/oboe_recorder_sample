package com.sheraz.oboerecorder

import android.content.Context
import android.content.Intent
import android.net.Uri
import android.os.Build
import android.provider.Settings
import android.util.Log
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import java.io.File

object Utils {

    private val TAG = Utils::class.java.simpleName

    fun getAudioRecordingFilePath(context: Context): String {

        val imagePath = File(context.getExternalFilesDir(null), "audio")
        if (!imagePath.exists()) {
            val directoryCreationStatus = imagePath.mkdirs()
            Log.i(TAG, "getAudioRecordingFilePath: directoryCreationStatus: $directoryCreationStatus")
        }

        val newFile = File(imagePath, "oboe_recording.wav")
        val filePath = newFile.absolutePath

        Log.d(TAG, "getAudioRecordingFilePath: filePath: $filePath, fileExists: ${newFile.exists()}")

        if (newFile.exists()) {
            val deletionStatus = newFile.delete()
            Log.i(TAG, "getAudioRecordingFilePath: File already exists, delete it first, deletionStatus: $deletionStatus")
        }

        if (!newFile.exists()) {
            val fileCreationStatus = newFile.createNewFile()
            Log.i(TAG, "getAudioRecordingFilePath: fileCreationStatus: $fileCreationStatus")
        }

        return filePath
    }

    fun showPermissionsErrorDialog(context: Context) {

        Log.d(TAG, "showPermissionsErrorDialog(): ")

        if (!((context as AppCompatActivity).isFinishing)) {

            var dialogTheme = android.R.style.Theme_Holo_Light_Dialog

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                Log.i(TAG, "showPermissionsErrorDialog: All permissions are not allowed, Show error dialog, Build.VERSION.SDK_INT >= 23")
                dialogTheme = android.R.style.Theme_Material_Light_Dialog
            }

            val builder = AlertDialog.Builder(context, dialogTheme)

            builder.setTitle("Permissions")
            builder.setMessage("Need some permissions for app to work correctly")
            builder.setCancelable(false)

            builder.setPositiveButton("Go to Settings") { dialog, _ ->
                Log.d(TAG, "positiveButton::onClick: ")
                dialog.dismiss()
                openAppSettingsPage(context)
            }

            builder.setNegativeButton("Cancel") { dialog, _ ->
                Log.d(TAG, "negativeButton::onClick: ")
                dialog.dismiss()
            }

            builder.show()
        }

    }

    fun openAppSettingsPage(context: Context) {
        val intent = Intent()
        intent.action = Settings.ACTION_APPLICATION_DETAILS_SETTINGS
        val uri = Uri.fromParts("package", context.packageName, null)
        intent.data = uri
        context.startActivity(intent)
    }
}