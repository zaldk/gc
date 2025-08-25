package main

import (
	"fmt"
	"io/fs"
	"os"
	fp "path/filepath"
)

type DatabaseHeader = struct {
	magic          uint64
	version        byte
	hash_id        byte
	compression_id byte
	encryption_id  byte
	_              [4]byte
}
type Database = struct {
	header    DatabaseHeader
	snapshots []Snapshot
}
type SnapshotHeader = struct {
	hash            uint64
	timestamp       uint64
	name_length     uint16
	email_length    uint16
	message_length  uint32
	snapshot_length uint64
	name            []byte
	email           []byte
	message         []byte
}
type Snapshot = struct {
	header SnapshotHeader
	files  []File
}
type File = struct {
	metadata_length uint32
	path_length     uint32
	content_length  uint32
	metadata_format byte
	metadata        []byte
	path            []byte
	content         []byte
}

type _file = struct {
	path string
	info fs.FileInfo
}

func main() {
	files := make([]_file, 0)
	fp.Walk("./src", func(path string, info fs.FileInfo, err error) error {
		if !info.IsDir() {
			files = append(files, _file{path, info})
		}
		return err
	})
	for _, f := range files {
		fmt.Printf("%v", f.path)
		data, err := os.ReadFile(f.path)
		if err != nil {
			return
		}
		fmt.Printf("\t`%s`\n", data)
	}
}

// func _main() {
// 	if len(os.Args) < 2 {
// 		print_usage()
// 		return
// 	}
// 	switch os.Args[1] {
// 	case "help":
// 		print_usage()
// 	case "init":
// 		gc_init()
// 	case "save":
// 		switch len(os.Args) {
// 		case 2:
// 			gc_save("")
// 		case 3:
// 			gc_save(os.Args[2])
// 		default:
// 			print_usage()
// 			return
// 		}
// 	case "log":
// 		switch len(os.Args) {
// 		case 3:
// 			gc_log(os.Args[2])
// 		default:
// 			print_usage()
// 			return
// 		}
// 	case "config":
// 		switch len(os.Args) {
// 		case 2:
// 			gc_config("", "")
// 		case 4:
// 			switch os.Args[2] {
// 			case "-n":
// 				gc_config(os.Args[3], "")
// 			case "-e":
// 				gc_config("", os.Args[3])
// 			default:
// 				print_usage()
// 				return
// 			}
// 		case 6:
// 			name_arg, email_arg := "", ""
// 			switch os.Args[2] {
// 			case "-n":
// 				name_arg = os.Args[3]
// 			case "-e":
// 				email_arg = os.Args[3]
// 			default:
// 				print_usage()
// 				return
// 			}
// 			switch os.Args[4] {
// 			case "-n":
// 				name_arg = os.Args[5]
// 			case "-e":
// 				email_arg = os.Args[5]
// 			default:
// 				print_usage()
// 				return
// 			}
// 			gc_config(name_arg, email_arg)
// 		default:
// 			print_usage()
// 			return
// 		}
//
// 	// TODO:
// 	case "clone":
// 		fmt.Printf("TODO: clone\n")
// 	case "push":
// 		fmt.Printf("TODO: push\n")
// 	case "pull":
// 		fmt.Printf("TODO: pull\n")
// 	case "remote":
// 		switch len(os.Args) {
// 		case 2:
// 			fmt.Printf("TODO: remote get\n")
// 		case 3:
// 			fmt.Printf("TODO: remote set %v\n", os.Args[2])
// 		default:
// 			print_usage()
// 			return
// 		}
// 	case "export":
// 		fmt.Printf("TODO: export\n")
// 	default:
// 		print_usage()
// 		return
// 	}
// }
//
// func gc_init()                     {}
// func gc_save(message string)       {}
// func gc_log(filepath string)       {}
// func gc_config(name, email string) {}
//
// func print_usage() {
// 	fmt.Printf(`Garbage Collector - a source control system that just does what its told.
// Commands:
// 	help				prints this help text.
// 	init				initializes a .gc directory, where all gc-related data will be stored.
// 	save [ "MESSAGE" ]		saves the current state with an optional message.
// 	log FILEPATH			logs the saves of the file specified by FILEPATH.
// 	config				prints the current user's config to the stdout.
// 	config [ -n NAME ] [ -e EMAIL ]	sets the current user's name and/or email.
// 	clone URL			clones the repository from the specified URL into the current directory.
// 	push				sends the last saved state state to the remote.
// 	pull				requests the latest state from the remote.
// 	remote				prints the current remote to the stdout.
// 	remote URL			sets the remote to the provided value.
// 	export FORMAT PATH		export the project to PATH as another FORMAT: git, fossil, mercurial, etc.
// `)
// }

func assert(cond bool, msg ...string) {
	if !cond {
		for _, m := range msg {
			fmt.Print(m)
		}
		fmt.Println()
		panic("fuck you")
	}
}
