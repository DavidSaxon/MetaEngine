#!/usr/bin/env python

# this script is a prototype for what Sigma will be able to do..

import os
import shutil
import sys


# --- globals ---

ARGS = [ 'all', 'libs', 'deploy' ]
TEST_LOG_PATH = 'logs/tests/log.xml'
BUILD_DIR = 'build'
SRC_DIR = 'src/cxx'

DIVIDER = '-' * 80


# --- read args ---

if len( sys.argv ) > 2:
    print( 'ERROR: No more than one build argument may be provided.' )
    exit()

mode = ARGS[ 0 ]
if len( sys.argv ) > 1:
    mode = sys.argv[ 1 ]

if not mode in ARGS:
    print( 'ERROR: Unknown mode: {0}.'.format( mode ) )
    exit()


# --- compile ---

if mode != 'deploy':

    print( DIVIDER )
    print( '                                    compiling...' )
    print( DIVIDER )

    # TODO: compile for windows

    make_command = 'make '

    os.system( make_command )

# --- deploy ---

if mode == 'all' or mode == 'deploy':

    print( DIVIDER )
    print( '                                    deploying...' )
    print( DIVIDER )


    # get semantic version
    major = 0
    minor = 0
    patch = 0
    with open( 'semver', 'r' ) as f:
        for line in f:
            if line.startswith( 'MAJOR' ):
                major = int( line.split( ' ' )[ 1 ] )
            elif line.startswith( 'MINOR' ):
                minor = int( line.split( ' ' )[ 1 ] )
            elif line.startswith( 'PATCH' ):
                patch = int( line.split( ' ' )[ 1 ] )

    # increment patch
    patch += 1

    # generate deployment name name
    deploy_name = 'MetaEngine-{0}.{1}.{2}'.format( major, minor, patch )
    deploy_location = 'deploy/' + deploy_name

    # does it exist?
    if os.path.exists( deploy_location ):
        print( 'ERROR: deployment directory already exists. Halting.' )
        exit()

    os.makedirs( deploy_location )

    # create the lib directory
    libs_location = deploy_location + '/lib'
    os.makedirs( libs_location )

    # create linux lib director
    linux_libs_location = libs_location + '/linux_x86'
    os.makedirs( linux_libs_location )

    # move libs in
    linux_build_dir = BUILD_DIR + '/linux_x86'
    for f in os.listdir( linux_build_dir ):
        if f.endswith( '.so' ) or f.endswith( '.lib' ):
            shutil.copy( linux_build_dir + '/' + f, linux_libs_location )

    # create linux lib director
    win_libs_location = libs_location + '/win_x86'
    os.makedirs( win_libs_location )

    # move libs in
    win_build_dir = BUILD_DIR + '/win_x86'
    for f in os.listdir( win_build_dir ):
        if f.endswith( '.so' ) or f.endswith( '.lib' ):
            shutil.copy( win_build_dir + '/' + f, win_libs_location )

    # create the include directory
    include_location = deploy_location + '/include'
    os.makedirs( include_location )

    # traverse src dir and copy headers
    for root, dirs, files in os.walk( SRC_DIR ):
        current_dir = include_location + root[ len( SRC_DIR ) : ]
        # create the directories
        for d in dirs:
            os.makedirs( current_dir + '/' + d )
        # copy files
        for f in files:
            if not f.startswith('__') and
               (f.endswith('.hpp') or f.endswith('.inl')):
                shutil.copy( root + '/' + f, current_dir )

    # create a meta file with the version number
    version_meta_path = deploy_location + '/Version: {0}.{1}.{2}'.format(
            major, minor, patch)
    version_meta = open(version_meta_path, 'a')
    version_meta.write('.\n\n')
    version_meta.close()

    # write back to semantic version file
    with open( 'semver', 'w' ) as f:
        f.write( 'MAJOR {0}\n'.format( major ) )
        f.write( 'MINOR {0}\n'.format( minor ) )
        f.write( 'PATCH {0}\n'.format( patch ) )

    print( '\nDEPLOYMENT NAME: {0}\n'.format( deploy_name ) )


print( DIVIDER )
print( '                                      done' )
print( DIVIDER )
