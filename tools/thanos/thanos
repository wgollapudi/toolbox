#!/bin/bash
# thanos.sh - A tool to randomly delete files. (brainrot)
#
# Usage:
#   thanos [-n <number> | -p <percentage>] [optional: -d <directory>, -m <regex>, -r] [--dry-run] [--confirm] [-h]
#
# Options:
#   -h              Show this help message.
#   -n number       Number of files to delete (mutually exclusive with -p).
#   -p percentage   Percentage of matching files to delete (mutually exclusive with -n).
#   -d directory    Directory to operate on (default: current directory).
#   -m regex        Regex to filter file names (or relative paths if -r is specified).
#   -r              Recursive mode: process files in subdirectories as well.
#   --dry-run       Show which files would be deleted, but do not delete them.
#   --confirm       Ask for confirmation before deleting each file.

if [ "$#" -eq 0 ]; then
    cat <<'EOF'
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⣿⣿⣿⣿⣿⣿⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⢿⣿⣿⣿⣿⣿⣿⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣾⣿⣿⣿⣿⣿⡿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⠹⡛⡷⠸⠟⠀⠀⠀⣷⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⡏⠉⠉⠉⠉⢰⡇⠀⠀⢀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢒⣶⡄⣾⣶⡼⢲⣿⠀⠐⢿⣿⣿⠇⠀⡀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⡿⠟⢉⣴⣬⠉⡀⠀⠰⠈⠁⡀⠀⠇⠀⠀⠀⠀⢀⣰
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢘⠋⠀⠀⠿⠏⠛⠀⠋⠀⠀⠀⠰⡇⢀⠀⠀⢰⣦⠀⠛⠁
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡧⠀⣴⣦⣈⠛⠿⢟⡸⣶⢿⠀⠀⣼⠀⣇⠀⠉⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢳⣧⢻⣿⡟⡍⣏⢸⡇⢿⢸⠘⠀⢸⠀⢿⡆⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠈⠻⡆⣿⣇⡃⣿⠸⣧⢸⠘⠀⠀⠀⠀⢸⡇⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⢠⣴⣿⣷⡀⠸⢹⣯⡇⣿⡆⣿⠀⠀⢰⡀⠀⠀⠈⠁⠀⠀⠀⠀
⠀⠀⠀⠀⢀⣤⣾⡇⢸⣿⣿⣿⣿⣀⠙⠇⠓⠘⠁⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⣀⣾⣿⣿⣿⡲⡀⠻⣿⣿⣿⣿⣷⣤⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⢠⣾⢟⣿⣿⠻⠟⠘⡶⡄⠙⣿⣿⣿⣿⣿⣿⢿⣶⣤⣤⣤⣤⣤⣤⣤⡄⠀⠀⠀⠀⠀
⠸⡿⠟⠿⢿⠇⠤⠁⢴⠈⠂⡈⠙⠻⢿⣿⣿⣄⠛⣹⣿⣿⣿⠟⠛⠉⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠽⠇⠀⠀⠁⠸⠢⠚⠁⠀⠀⠀⠀⠈⠉⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
EOF
    exit 0
fi

usage() {
    cat <<EOF
Usage: $0 [-n <number> | -p <percentage>] [optional: -d <directory>, -m <regex>, -r] [--dry-run] [--confirm] [-h]

Options:
  -h              Show this help message.
  -n number       Number of files to delete (mutually exclusive with -p).
  -p percentage   Percentage of matching files to delete (mutually exclusive with -n).
  -d directory    Directory to operate on (default: current directory).
  -m regex        Regex to filter file names (or relative paths if -r is specified).
  -r              Recursive mode: process files in subdirectories as well.
  --dry-run       Show which files would be deleted, but do not delete them.
  --confirm       Ask for confirmation before deleting each file.
EOF
}

delete_number=""
delete_percentage=""
directory="."
regex=""
recursive=false
dry_run=false
confirm=false

while [[ $# -gt 0 ]]; do
    case "$1" in
        -h|--help)
            usage
            exit 0
            ;;
        -n)
            if [[ -n "$delete_percentage" ]]; then
                echo "Error: -n and -p are mutually exclusive."
                exit 1
            fi
            shift
            if [[ $# -eq 0 ]]; then
                echo "Error: -n requires a numeric argument."
                exit 1
            fi
            delete_number="$1"
            ;;
        -p)
            if [[ -n "$delete_number" ]]; then
                echo "Error: -n and -p are mutally exclusive."
                exit 1
            fi
            shift
            if [[ $# -eq 0 ]]; then
                echo "Error: -p requires a numeric argument."
                exit 1
            fi
            delete_percentage="$1"
            ;;
        -d)
            shift
            if [[ $# -eq 0 ]]; then
                echo "Error: -d requires a directory argument."
                exit 1
            fi
            directory="$1"
            ;;
        -m)
            shift
            if [[ $# -eq 0 ]]; then
                echo "Error: -m requires a regex argument."
                exit 1
            fi
            regex="$1"
            ;;
        -r)
            recursive=true
            ;;
        --dry-run)
            dry_run=true
            ;;
        --confirm)
            confirm=true
            ;;
        *)
            echo "Unknown option: $1"
            usage
            exit 1
            ;;
    esac
    shift
done

if [[ -z "$delete_number" && -z "$delete_percentage" ]]; then
    echo "Error: You must specify either -n <number> or -p <percentage>."
    usage
    exit 1
fi

if [[ "$delete_number" == "0" ]]; then
    delete_number=""
fi
if [[ "$delete_percentage" == "0" ]]; then
    delete_percentage=""
fi

if [[ ! -d "$directory" ]]; then
    echo "Error: Directory '$directory' does not exist."
    exit 1
fi

find_args=()
find_args+=("$directory")
if [[ "$recursive" == false ]]; then
    find_args+=("-maxdepth" "1")
fi
find_args+=("-type" "f")

if [[ -n "$regex" ]]; then
    if [[ "$recursive" == true ]]; then
        files=( $(find "${find_args[@]}" | sed "s|^$directory/||" | grep -E "$regex") )
        for i in "${!files[@]}"; do
            files[$i]="$directory/${files[$i]}"
        done
    else
        files=( $(find "${find_args[@]}" -printf "%f\n" | grep -E "$regex") )
        # Prepend directory path back.
        for i in "${!files[@]}"; do
            files[$i]="$directory/${files[$i]}"
        done
    fi
else
    if [[ "$recursive" == true ]]; then
        files=( $(find "${find_args[@]}") )
    else
        files=( $(find "${find_args[@]}" -printf "%f\n") )
        for i in "${!files[@]}"; do
            files[$i]="$directory/${files[$i]}"
        done
    fi
fi

total_files=${#files[@]}
if [[ $total_files -eq 0 ]]; then
    echo "Error: No matching files found in '$directory'."
    exit 1
fi

if [[ -n "$delete_number" ]]; then
    target_count=$delete_number
else
    target_count=$(echo "$total_files * $delete_percentage / 100" | bc -l | awk '{printf "%d", ($1 < 1 ? 1 : $1)}')
fi

if (( target_count > total_files )); then
    target_count=$total_files
fi

echo "Found $total_files matching files."
echo "Target to delete: $target_count files."

selected_files=( $(printf "%s\n" "${files[@]}" | shuf -n "$target_count") )

for file in "${selected_files[@]}"; do
    # If --confirm is enabled, ask for confirmation.
    if [[ "$confirm" == true ]]; then
        read -p "Delete file '$file'? [y/N]: " answer
        case "$answer" in
            [yY][eE][sS]|[yY])
                ;;
            *)
                echo "Skipping '$file'"
                continue
                ;;
        esac
    fi

    if [[ "$dry_run" == true ]]; then
        echo "[Dry Run] Would delete: $file"
    else
        rm -- "$file"
        if [[ $? -eq 0 ]]; then
            echo "Deleted: $file"
        else
            echo "Error deleting: $file"
        fi
    fi
done

echo "Operation completed."
