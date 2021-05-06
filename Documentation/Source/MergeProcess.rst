Merge Process
=============

Quick-steps
-----------

#. Pull all changes from ``origin/master``.
#. Create a new local branch from ``origin/master``.
#. Implement your changes and commit to your branch, pushing to ``origin`` periodically.
#. When all of your changes are complete, pull all changes from ``origin/master`` to ``local/master``.
#. Rebase your local branch onto ``local/master``.
#. Resolve any conflicts in your branch.
#. Push your branch to the remote server.

    * Repeat the last four steps as necessary for your development.

#. Create a Merge Request in GitLab to merge your branch into ``origin/master``.

    * The merge request will tell you if you need to rebase or perform other work to get synchronized.

#. Checkout the merging branch locally.
#. Clean your solution **for both Debug and Release**.

    * In Visual Studio, you can use the ``Build/Batch Build...`` command to simplify this process.
    
#. Build your solution **for both Debug and Release**.
#. Run Bootstrap **for both Debug and Release**.

    * Verify the DigiPen splash screen appears.
    * Verify the menu appears after the first 5 seconds.
    * Verify the 'Enter' key brings up the game.

#. Run Editor **for both Debug and Release**.

    * Verify you can open the ProjectBoomerang.prj file.
    * Verify you can open the StressTest scene.
    * Verify you can see the Gizmo when you select an object in the hierarchy
    * Move the camera (pan, zoom, nudge) to see the object.
    * Verify you can add, modify and delete components.
    * You do not need to save the scene.

#. If all verification checks pass, in GitLab you may Merge the merge request.

    * The merge request will close automatically.


