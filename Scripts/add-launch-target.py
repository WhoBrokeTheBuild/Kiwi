#!/usr/bin/env python3

import os
import json
import argparse
import platform

parser = argparse.ArgumentParser()

parser.add_argument(
    '--name',
    required=True,
    help='Name of the config target to generate.'
)

parser.add_argument(
    '--executable',
    required=True,
    help='Path to executable to run.'
)

parser.add_argument(
    '--runtimePathList',
    required=True,
    help='Semicolon-delimted list of paths to search for dll/so files, will be used in PATH or LD_LIBRARY_PATH.'
)

parser.add_argument(
    '--binaryDir',
    required=True,
    help='CMAKE_BINARY_DIR, will be used with executable.'
)

parser.add_argument(
    '--workingDir',
    required=True,
    help='Directory to run the executable from.'
)


parser.add_argument(
    'arguments',
    nargs='*',
    help='Additional arguments to pass to the executable.'
)

args = parser.parse_args()

name = args.name
executable = args.executable
arguments = args.arguments
runtimePathList = args.runtimePathList
binaryDir = args.binaryDir
workingDir = args.workingDir

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

config = {}
try:
    file = open(filename, 'r')
    config = json.load(file)
except:
    pass

if 'version' not in config:
    config['version'] = '0.2.0'

if 'configurations' not in config:
    config['configurations'] = []

data = {
    'name': name,
    'type': 'cppdbg',
    'request': 'launch',
    'program': executable,
    'args': arguments,
    'cwd': workingDir,
    'environment': []
}

if platform.system() == 'Windows':
    data['type'] = 'cppvsdbg'
    data['environment'].append({
        'name': 'PATH',
        'value': '${env:PATH};' + runtimePathList
    })
    data['console'] = 'integratedTerminal'
else:
    data['environment'].append({
        'name': 'LD_LIBRARY_PATH',
        'value': runtimePathList
    })
    data['externalConsole'] = False

if platform.system() == 'Darwin':
    data['MIMode'] = 'lldb'

add_or_update_config(data, config['configurations'])

file = open(filename, 'w')
json.dump(config, file, indent=4)

###
### Visual Studio
###

if platform.system() == 'Windows':
    if not os.path.isdir('.vs'):
        os.mkdir('.vs')
    
    filename = '.vs/launch.vs.json'

    config = {}
    try:
        file = open(filename, 'r')
        config = json.load(file)
    except:
        pass

    if 'version' not in config:
        config['version'] = '0.2.1'

    if 'configurations' not in config:
        config['configurations'] = []

    data = {
        'name': name,
        'type': 'default',
        'project': 'CMakeLists.txt',
        'args': arguments,
        'projectTarget': '{} ({})'.format(
            os.path.basename(executable),
            os.path.relpath(executable, binaryDir).replace('/', '\\')
        ),
        'cwd': workingDir,
        'env': {
            'PATH': '${env.PATH};' + runtimePathList
        },
    }

    add_or_update_config(data, config['configurations'])

    file = open(filename, 'w')
    json.dump(config, file, indent=4)
