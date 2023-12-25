import re,sys

from street_graph import Street_Graph

def main():
    streets_data = {}

    def normalize_street_name(street_name):
        street_name = street_name.replace('"', '')
        street_name = street_name.strip()
        street_name = str(street_name).lower()
        return street_name

    while True:
        try:
            input_string = input()
        except EOFError:
            sys.exit(0)
        if input_string == '':
            sys.exit(0)

        pattern = r'(add|mod|rm|gg)((?: +?"[a-zA-Z0-9 ]+?" *?))?((?:\([-]?[0-9]+?,[-]?[0-9]+?\) *?)*)?$'
        regex_input = re.match(pattern, input_string)

        if regex_input:
            command, street_name, coorlist = regex_input.group(1), regex_input.group(2), regex_input.group(3)

            if command in ["add", "mod", "rm"] and len(street_name) < 1:
                sys.stderr.write("Error! Street name is missing\n")
                continue
            
            elif command in ["add", "mod", "rm"] and len(street_name)>=1:
                street_name = normalize_street_name(street_name)
    
            coorlist = re.findall(r'\([-]?[0-9]+,[-]?[0-9]+\)', coorlist)

            if command in ["add", "mod"] and len(coorlist) < 2:
                sys.stderr.write("Error! Minimum of 2 coordinates required\n")
                continue   

        else:
            sys.stderr.write("Error: The input is not according to the regex pattern\n")
            continue

        try:
            if command == 'add':
                if street_name in streets_data:
                    sys.stderr.write("Error: Street name " + street_name + " already present!\n")
                    continue

                streets_data[street_name] = coorlist

            elif command == 'mod':
                street_name = normalize_street_name(street_name)

                if street_name not in streets_data:
                    sys.stderr.write("Error: Street Name that doesn't exist!\n")
                    continue

                streets_data[street_name] = coorlist

            elif command == 'rm':
                street_name = normalize_street_name(street_name)

                if street_name not in streets_data:
                    sys.stderr.write("Error: Street Name that doesn't exist\n")
                    continue

                del streets_data[street_name]

            elif command == 'gg':
                if len(streets_data) == 0:
                    sys.stderr.write("Error: No streets entered\n")
                    continue
                Street_Graph(streets_data).generate_graph()

            else:
                sys.stderr.write("Error: Invalid Command\n")

        except Exception as exp:
            sys.stderr.write("Error: " + str(exp) + "\n")
            pass
    # return exit code 0 on successful termination
    sys.exit(0)

if __name__ == '__main__':
    main()
