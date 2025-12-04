# GC C API

## Modules

### Filesystem
OS-dependent

This will handle the filesystem of the user's os.
Will need to:
- reserve page-sized blocks of hard drive space
- extend files in page-sizes
- expose an api to store data as pages
- ensure atomicity:
    All operations will happen in a secondary, lesser DB, with 2 copies of the
    original's affected data pages - one for editing and one for reverting.
    Then, once the data storage is done, merge the lesser's delta into the primary DB.
Will not:
- handle *smart* storage - this is the logic's responsibility

### Network
OS-independent* - work will be done through third parties.

This will handle the data transfer over:
1. insecure HTTP at 0.x
2. HTTPS and SSH at 1.0

As well as a HTTP server with a simple web UI at 0.x
And a fully capable web interface as good as a cli at 1.0

### Logic
OS-independent

Will be responsible for:
1. smart storage and retrieval from local FS at 0.x and dumb over network
2. smart storage and retrieval from both local and network at 1.0
3. making the layout for the data stored by GC - files+paths, contibutors, commits, etc.

### CLI
OS-independent

Will handle:
- parsing the commands and options from the command line
- storing and retrieving config data

## Details

### CLI

$ gc save SRC
$ gc pull SRC
$ gc push DST
$ gc config (global | default: local)
$ gc log
