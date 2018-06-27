package threads.main;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

import threads.calculateE.CalculateE;
import threads.calculateE.StartThreads;
import threads.gui.GUI;

public class Main {

    public static void main(String[] args) {
        CommandLineParser parser = new DefaultParser();
        Options options = new Options();
        options.addOption("q", "if selected, GUI won't be loaded");
        options.addOption("p", true, "specifies how many iterations to be made during calculation");
        options.addOption("t", true, "specifies how many threads should be used for calculation");
        options.addOption("o", true, "specifies а file in which the test results will be saved");

        int threadCount = 8;
        int precision = 250;
        String output = "result.txt";

        try {
            CommandLine line = parser.parse(options, args);

            if (line.hasOption("p")) {
                precision = Integer.parseInt(line.getOptionValue("p"));
            }
            if (line.hasOption("t")) {
                threadCount = Integer.parseInt(line.getOptionValue("t"));
            }
            if (line.hasOption("o")) {
                output = line.getOptionValue("o");
            }

            CalculateE.precision = precision;

            if (line.hasOption("q")) {
                StartThreads.start(threadCount, null, output);
            } else {
                GUI gui = new GUI(output);
            }

        } catch (ParseException exp) {
            System.out.println("Unexpected exception:" + exp.getMessage());
        }
    }

}
