# By Laurentiu Stefan 343C3

function compare_files {
    diff $1 $2
    return $?
}

pad=$(printf '%.1s' "."{1..70})
padlength=71

function print_result {
    printf "%s" "$1"
    printf "%*.*s" 0 $((padlength - ${#1} - ${#2} )) "$pad"
    printf "%s\n" "$2"
}

function check_prerequisites {
    # Check if makefile exists
    ls . | grep -i "makefile" &> /dev/null

    if [ $? -ne 0 ]; then
        echo -e "Makefile file not found! Stopping testing!"
        exit 1
    fi

    makefile_name=$(ls . | grep -i makefile | cut -f 1 -d " ")

    # Check for makefile build, run, clean rules
    cat $makefile_name | grep -i "build:" &> /dev/null
    if [ $? -ne 0 ]; then
        echo -e "[MAKEFILE]: BUILD rule not found! Stopping testing!"
        exit 1
    fi

    cat $makefile_name | grep -i "clean:" &> /dev/null
    if [ $? -ne 0 ]; then
        echo -e "[MAKEFILE]: CLEAN rule not found! Stopping testing!" exit 1
    fi
}

function check_tasks {
    total_points=0
    tests_weight=(7 7 7 7 7 7 7 7 7 7)

    make clean &> /dev/null
    make build 

    if [ $? -ne 0 ]; then
        echo "[MAKEFILE] Makefile build rule failed! Stopping testing!"
        exit 1
    fi

    # Check if the executable exists
    ls . | grep "editor" &> /dev/null

    if [ $? -ne 0 ]; then
        echo -e "editor executable not found! Stopping testing!"
        exit 1
    fi

    echo "===========================TESTS================================"

    for i in $(seq 1 10)
    do
        current_description=""

        if [ $i -lt 7 ]; then
            current_description="(touch mkdir ls)"
        elif [ $i -lt 11 ]; then
            current_description="(pwd cd)"
        elif [ $i -lt 13 ]; then
            current_description="(tree)"
        else
            current_description="(rm rmdir)"
        fi

        current_input="./input/editor${i}.in"
        current_output="./output/editor${i}.out"
        current_ref="./ref/editor${i}.ref"
	
        # Running the executable
        touch editor.out
        cp $current_input editor.in
        timeout 1 ./editor 
        cp editor.out $current_output

       	
        # Compare the output with the reference
        compare_files $current_output $current_ref

        if [ $? -eq 0 ]; then
            print_result "Test $i" "OK (${tests_weight[i - 1]}p)"
            ((total_points=total_points+${tests_weight[i - 1]}))
        else
            print_result "Test $i" "FAILED"
        fi
        
	    timeout 3 valgrind --tool=memcheck --leak-check=full --error-exitcode=1 ./editor >& /dev/null

	    if [ $? -eq 0 ]; then
		print_result "Test valgrind $i" "OK (2p)"
		((total_points=total_points+2))
	    else
		print_result "Test valgrind $i" "FAILED"
	    fi
    done

    # Running the valgrind test
    

    echo ""
    print_result "Total TESTS" "[${total_points}p / 90p]"
    ((total_points=total_points+current_test_points))

    echo "======================================================================="
    echo ""

    make clean &> /dev/null
    #rm output/* &> /dev/null

    #rm editor.in
    #rm editor.out
}

function check_readme {
    echo "=============================README===================================="

    ls . | grep -i "readme" &> /dev/null

    if [ $? -eq 0 ]; then
        print_result "Checking for README file" "OK (10p for now)"
        readme_points=10
        ((total_points=total_points+10))
    else
        readme_points=0
        print_result "Checking for README file" "FAILED"
    fi

    echo ""
    print_result "Total README" "[${readme_points}p / 10p]"
    echo "======================================================================="
    echo ""
}


# "Main" code starts here
check_prerequisites

check_tasks

check_readme

echo "TOTAL: ${total_points}/100"
