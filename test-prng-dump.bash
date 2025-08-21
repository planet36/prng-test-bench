#!/usr/bin/bash
# SPDX-FileCopyrightText: Steven Ward
# SPDX-License-Identifier: OSL-3.0

# XXX: If "Broken pipe" error happens, it's likely due to out-of-memory.

renice --priority 19 --pid $$ > /dev/null

export LC_ALL=C

SCRIPT_NAME="$(basename -- "${BASH_SOURCE[0]}")"
#SCRIPT_DIR="$(dirname -- "${BASH_SOURCE[0]}")"

function program_exists
{
    # https://pubs.opengroup.org/onlinepubs/9699919799/utilities/command.html
    command -v "${@}" > /dev/null
}

function is_val_in_array
{
    local -r VAL="$1"
    shift || return

    for x
    do
        if [[ "$VAL" == "$x" ]]
        then
            return 0 # success
        fi
    done
    return 1 # failure
}

declare -r PRNG_DUMP_INFO_FILE="$XDG_RUNTIME_DIR/prng-dump-info.txt"
declare -r PRNG_DUMP_CMDS_FILE="$XDG_RUNTIME_DIR/prng-dump-cmds.sh"

declare -r -a VALID_SEED_TYPES=(default pattern random zero)
declare -r DEFAULT_SEED_TYPE=default

# Passed to RNG_test

declare -r -a VALID_TFS=(0 1 2)
declare -r DEFAULT_TF=1

declare -r DEFAULT_TLMIN=256MB

declare -r DEFAULT_TLMAX=1GB

function print_help
{
    printf 'Usage: %q [OPTION]... [PRNGs]\n' "$SCRIPT_NAME"
    cat <<EOT

Test prng-dump with RNG_test.  If PRNGs are not given, they are all tested.

Run "./prng-dump -i" to list all PRNGs.

The prng-bench target must be run before this script for benchmark analysis to be done.

OPTIONS

  -h : Print this message, then exit.

  -d : Pass the '--dry-run' option to parallel.

  -f TF : Specify the value of the '-tf' option passed to RNG_test.
     If not given, $DEFAULT_TF is the default value.
     Must be at least $DEFAULT_TF.  See "RNG_test -help" for details.
     Valid values are: ${VALID_TFS[@]}

  -j JOBS : Specify the value of the '--jobs' option passed to parallel.
     If not given, no such option is passed to parallel.

     Possible values of JOBS are:
       1
       2
       50% # Limit processors to nproc/2
       -1  # Limit processors to nproc-1
       +0  # Limit processors to nproc

     Consider that the '-multithreaded' option is passed to RNG_test.
     See "man parallel" for details.
     <https://www.gnu.org/software/parallel/parallel.html>

  -m TLMAX : Specify the value of the '-tlmax' option passed to RNG_test.
     If not given, $DEFAULT_TLMAX is the default value.
     Must be at least $DEFAULT_TLMIN.  See "RNG_test -help" for details.

  -s SEED_TYPE : Specify the seed type given to the '-s' option passed to prng-dump.
     This option may be given more than once.
     If not given, $DEFAULT_SEED_TYPE is the default value.
     Valid values are: ${VALID_SEED_TYPES[@]}

EOT
}

declare -a SEED_TYPES=()

# Passed to RNG_test

TF="$DEFAULT_TF"

TLMIN="$DEFAULT_TLMIN"

TLMAX="$DEFAULT_TLMAX"

# Passed to RNG_test
declare -r MULTITHREADED='-multithreaded'

while getopts 'dhf:j:m:s:' option
do
    case "$option" in
        d)
            # Passed to parallel
            DRY_RUN='--dry-run'
            ;;

        h)
            print_help
            exit
            ;;

        f)
            if ! is_val_in_array "${OPTARG}" "${VALID_TFS[@]}"
            then
                printf 'Error: Invalid value for -%s: %q\n' \
                    "$option" "$OPTARG" 1>&2
                printf 'Try "%q -h" for more information.\n' "$SCRIPT_NAME" 1>&2
                exit 1
            fi
            # Passed to RNG_test
            TF="${OPTARG}"
            ;;

        j)
            # Passed to parallel
            JOBS="--jobs=${OPTARG}"
            ;;

        m)
            # Passed to RNG_test
            TLMAX="${OPTARG}"
            ;;

        s)
            if ! is_val_in_array "${OPTARG}" "${VALID_SEED_TYPES[@]}"
            then
                printf 'Error: Invalid value for -%s: %q\n' \
                    "$option" "$OPTARG" 1>&2
                printf 'Try "%q -h" for more information.\n' "$SCRIPT_NAME" 1>&2
                exit 1
            fi
            # Passed to prng-dump
            SEED_TYPES+=("${OPTARG}")
            ;;

        *) exit 1
            ;;
    esac
done

shift $((OPTIND - 1))

# If no seed types were given
if ((${#SEED_TYPES[@]} == 0))
then
    SEED_TYPES+=("$DEFAULT_SEED_TYPE")
fi

if ! program_exists RNG_test
then
    printf 'Error: RNG_test not found\n' 1>&2
    printf 'Try "%q -h" for more information.\n' "$SCRIPT_NAME" 1>&2
    exit 1
fi

if [[ ! -x prng-dump ]]
then
    printf 'Error: prng-dump not executable\n' 1>&2
    printf 'Try "%q -h" for more information.\n' "$SCRIPT_NAME" 1>&2
    exit 1
fi

if ! program_exists parallel
then
    printf 'Error: parallel not found\n' 1>&2
    printf 'Try "%q -h" for more information.\n' "$SCRIPT_NAME" 1>&2
    exit 1
fi

if (($# == 0))
then
    ./prng-dump -i > "$PRNG_DUMP_INFO_FILE" || exit
else
    truncate --size=0 -- "$PRNG_DUMP_INFO_FILE" || exit

    for PRNG
    do
        ./prng-dump -i | grep -w -F -- "$PRNG" >> "$PRNG_DUMP_INFO_FILE" || exit
    done
fi

declare -r OUTPUT_DIR=results

mkdir --verbose --parents -- "$OUTPUT_DIR" || exit

echo
echo "# prng-dump.opts="
fold -s < prng-dump.opts | sed -E -e 's/^/#   /'
echo
echo
echo "# nproc=$(nproc)"
echo "# PRNG_DUMP_INFO_FILE=$PRNG_DUMP_INFO_FILE"
echo "# PRNG_DUMP_CMDS_FILE=$PRNG_DUMP_CMDS_FILE"
echo
echo "# (passed to parallel)"
echo "# DRY_RUN=$DRY_RUN"
echo "# JOBS=$JOBS"
echo
echo "# (passed to prng-dump)"
echo "# SEED_TYPES=(${SEED_TYPES[*]})"
echo
echo "# (passed to RNG_test)"
echo "# TF=$TF"
echo "# TLMIN=$TLMIN"
echo "# TLMAX=$TLMAX"
echo "# MULTITHREADED=$MULTITHREADED"
echo

for SEED_TYPE in "${SEED_TYPES[@]}"
do
    echo "# Run each test with: seed=$SEED_TYPE, tf=$TF, tlmin=$TLMIN, tlmax=$TLMAX"
    echo

    # Does not include the PRNG name
    PRACTRAND_OUTFILE_STEM="RNG_test.tlmax-$TLMAX.seed-$SEED_TYPE"
    OUTFILE_STEM="prng-results.tlmax-$TLMAX.seed-$SEED_TYPE"

    while read -r PRNG_NAME PRNG_RESULT_SIZE_BITS _; do
        if ((PRNG_RESULT_SIZE_BITS > 64)) # RNG_test takes at most stdin64
        then
            PRNG_RESULT_SIZE_BITS=64
        fi
        printf "./prng-dump -s %s %s | RNG_test %s -tf %d -tlmin %s -tlmax %s %s > %q\n" \
            "$SEED_TYPE" "$PRNG_NAME" "stdin$PRNG_RESULT_SIZE_BITS" "$TF" \
            "$TLMIN" "$TLMAX" "$MULTITHREADED" "$OUTPUT_DIR/$PRACTRAND_OUTFILE_STEM.prng-$PRNG_NAME.txt"
    done < "$PRNG_DUMP_INFO_FILE" > "$PRNG_DUMP_CMDS_FILE"

    # shellcheck disable=SC2086
    /usr/bin/time --quiet --format='# Took %E\n' \
        parallel $DRY_RUN $JOBS '{}' < "$PRNG_DUMP_CMDS_FILE" > /dev/null || exit

    cd "$OUTPUT_DIR" || exit

    # Determine good and failed PRNGs in files.

    grep -l -L -F FAIL "$PRACTRAND_OUTFILE_STEM".prng-*.txt > \
        "$OUTFILE_STEM.files.good.txt"

    grep -l -F FAIL "$PRACTRAND_OUTFILE_STEM".prng-*.txt > \
        "$OUTFILE_STEM.files.failed.txt"

    # Extract their names.

    sed -e "s/^$PRACTRAND_OUTFILE_STEM\.prng-//" -e 's/\.txt$//' < \
        "$OUTFILE_STEM.files.good.txt" |
        sort > "$OUTFILE_STEM.names.good.txt" || exit

    sed -e "s/^$PRACTRAND_OUTFILE_STEM\.prng-//" -e 's/\.txt$//' < \
        "$OUTFILE_STEM.files.failed.txt" |
        sort > "$OUTFILE_STEM.names.failed.txt" || exit

    echo "# Good (seed $SEED_TYPE):"
    cat "$OUTFILE_STEM.names.good.txt" || exit
    echo

    echo "# Failed (seed $SEED_TYPE):"
    cat "$OUTFILE_STEM.names.failed.txt" || exit
    echo

    echo "# Time taken for RNG_test to reach $TLMAX (only for good generators (seed $SEED_TYPE)):"
    while IFS= read -r FILE
    do
        grep -H '^length= ' "$FILE" | tail -n 1 |
            sed -e "s/^$PRACTRAND_OUTFILE_STEM.prng-//" -e 's/.txt:/ /' || exit
    done < "$OUTFILE_STEM.files.good.txt" | column --table
    echo

    echo "# Failure happened after this length (seed $SEED_TYPE):"
    while IFS= read -r FILE
    do
        grep -H '^length= ' "$FILE" | tail -n 1 |
            sed -e "s/^$PRACTRAND_OUTFILE_STEM.prng-//" -e 's/.txt:/ /' || exit
    done < "$OUTFILE_STEM.files.failed.txt" | column --table
    echo

    cd - > /dev/null || exit
done

cd "$OUTPUT_DIR" || exit

# If default and random were given as seed types
if is_val_in_array default "${SEED_TYPES[@]}" && is_val_in_array random "${SEED_TYPES[@]}"
then
    echo "# Verify that generators with default construction and random seeds are equivalent."
    echo

    diff --unified \
        "prng-results.tlmax-$TLMAX.seed-default.names.failed.txt" \
        "prng-results.tlmax-$TLMAX.seed-random.names.failed.txt" #|| exit

    diff --unified \
        "prng-results.tlmax-$TLMAX.seed-default.names.good.txt" \
        "prng-results.tlmax-$TLMAX.seed-random.names.good.txt" #|| exit
fi

# Conclusion: no need to run random seed tests for simple failures

if [[ ! -f prng-bench.txt ]]
then
    printf 'Warning: prng-bench output file not found\n' 1>&2
    printf 'Try "%q -h" for more information.\n' "$SCRIPT_NAME" 1>&2
    exit 0
fi

echo
echo "# Performance all:"
sort -r -k 2 -g prng-bench.txt | column --table || exit
echo

{
printf 'q1\tmean\tmedian\tq3\tiqr\n'
datamash --whitespace --format '%.2f' q1 2 mean 2 median 2 q3 2 iqr 2 < prng-bench.txt || exit
} | column --table
echo

for SEED_TYPE in "${SEED_TYPES[@]}"
do
    # Does not include the PRNG name
    PRACTRAND_OUTFILE_STEM="RNG_test.tlmax-$TLMAX.seed-$SEED_TYPE"
    OUTFILE_STEM="prng-results.tlmax-$TLMAX.seed-$SEED_TYPE"

    echo "# Performance good (seed $SEED_TYPE):"
    grep --no-filename -w -E "$(paste -s -d '|' < "$OUTFILE_STEM.names.good.txt")" prng-bench.txt |
        sort -r -k 2 -g | column --table || exit
    echo

    {
    printf 'q1\tmean\tmedian\tq3\tiqr\n'
    grep --no-filename -w -E "$(paste -s -d '|' < "$OUTFILE_STEM.names.good.txt")" prng-bench.txt |
        datamash --whitespace --format '%.2f' q1 2 mean 2 median 2 q3 2 iqr 2 || exit
    } | column --table
    echo

    # Combine the RNG_test results and the benchmark results into a json file.

    TMP_FILE="$(mktemp)"

    while IFS= read -r FILE
    do
        PRNG="${FILE#$PRACTRAND_OUTFILE_STEM.prng-}"
        PRNG="${PRNG%.txt}"
        BYTES="$(grep '^length= ' "$FILE" | tail -n 1 | grep -o -P '(?<=\(2\^)[0-9]+(?= bytes\))')"
        FAILURE=false
        printf "%s\t%s\t%s\n" "$PRNG" "$BYTES" "$FAILURE"
    done < "$OUTFILE_STEM.files.good.txt" >> "$TMP_FILE"

    while IFS= read -r FILE
    do
        PRNG="${FILE#$PRACTRAND_OUTFILE_STEM.prng-}"
        PRNG="${PRNG%.txt}"
        BYTES="$(grep '^length= ' "$FILE" | tail -n 1 | grep -o -P '(?<=\(2\^)[0-9]+(?= bytes\))')"
        FAILURE=true
        printf "%s\t%s\t%s\n" "$PRNG" "$BYTES" "$FAILURE"
    done < "$OUTFILE_STEM.files.failed.txt" >> "$TMP_FILE"

    join -j 1 <(LC_ALL=C sort -k1 "$TMP_FILE") <(LC_ALL=C sort -k1 prng-bench.txt) |
        jq -R -f filter.jq | jq -s > "$OUTFILE_STEM.json"

    rm -f -- "$TMP_FILE"
done
