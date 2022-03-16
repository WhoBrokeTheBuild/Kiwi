#!/usr/bin/env python3

import os
import json
import argparse

isWindows = (os.name == 'nt')

parser = argparse.ArgumentParser()

parser.add_argument(
    '--name',
    required=True,
    help='Name of the launch target to generate.'
)

parser.add_argument(
    '--binaryDir',
    required=True,
    help='CMAKE_BINARY_DIR, will be used with executable.'
)

parser.add_argument(
    '--runtimePath',
    required=True,
    help='Semicolon-delimted list of runtime paths, will be used in PATH or LD_LIBRARY_PATH.'
)

parser.add_argument(
    '--executable',
    required=True,
    help='Path to executable to run.'
)

parser.add_argument(
    'arguments',
    nargs='*',
    help='Additional arguments to pass to the executable.'
)

args = parser.parse_args()

name       = args.name
executable = args.executable
arguments  = args.arguments
binaryDir  = args.binaryDir
rootDir    = os.getcwd()

# Replace with OS-specific multiple path separator (';' for PATH, ':' for LD_LIBRARY_PATH)
runtimePath = args.runtimePath

def add_or_update_config(data, configurations):
    found = False
    for i in range(0, len(configurations)):
        if configurations[i]['name'] == data['name']:
            configurations[i] = data
            found = True
            break
    
    if not found:
        configurations.append(data)

###
### Visual Studio Code
###

if not os.path.isdir('.vscode'):
    os.mkdir('.vscode')

filename = '.vscode/launch.json'

launch = {}
try:
    file = open(filename, 'r')
    launch = json.load(file)
except:
    pass

if 'version' not in launch:
    launch['version'] = '0.2.0'

if 'configurations' not in launch:
    launch['configurations'] = []

data = {
    'name': name,
    'type': 'cppdbg',
    'request': 'launch',
    'program': executable,
    'args': arguments,
    'cwd': rootDir,
    'environment': []
}

if isWindows:
    data['type'] = 'cppvsdbg'
    data['environment'].append({
        'name': 'PATH',
        'value': '${env:PATH};' + runtimePath
    })
    data['console'] = 'integratedTerminal'
else:
    data['environment'].append({
        'name': 'LD_LIBRARY_PATH',
        'value': runtimePath
    })
    data['externalConsole'] = False

add_or_update_config(data, launch['configurations'])

file = open(filename, 'w')
json.dump(launch, file, indent=4)

###
### Visual Studio
###

isWindows = True
if isWindows:
    if not os.path.isdir('.vs'):
        os.mkdir('.vs')
    
    filename = '.vs/launch.vs.json'

    

    launch = {}
    try:
        file = open(filename, 'r')
        launch = json.load(file)
    except:
        pass

    if 'version' not in launch:
        launch['version'] = '0.2.1'

    if 'configurations' not in launch:
        launch['configurations'] = []

    data = {
        'name': name,
        'type': 'default',
        'project': 'CMakeLists.txt',
        'args': arguments,
        'projectTarget': '{} ({})'.format(
            os.path.basename(executable),
            os.path.relpath(executable, binaryDir).replace('/', '\\')
        ),
        'cwd': rootDir,
        'env': {
            'PATH': '${env.PATH};' + runtimePath
        },
    }

    add_or_update_config(data, launch['configurations'])

    file = open(filename, 'w')
    json.dump(launch, file, indent=4)
