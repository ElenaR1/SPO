package E;

import java.io.FileNotFoundException;
import java.util.Scanner;
import java.io.PrintStream;
import java.util.Calendar;
import org.apfloat.Apfloat;
import org.apfloat.ApfloatMath;
import java.math.BigInteger;

public class E {

    static int numThread;
    static int precision;
    static String outputFile = "result.txt";
    static boolean quiet = false;
    static MyThread[] array;
    static Apfloat sum;

    public static void main(String[] args) throws FileNotFoundException {
        long timeOfStart = Calendar.getInstance().getTimeInMillis();
        for (int i = 0; i < args.length; i++) {
            if (args[i].equals("-p")) {
                precision = new Integer(args[i + 1]);
            }
            if (args[i].equals("-t") || args[i].equals("-task")) {
                numThread = new Integer(args[i + 1]);
            }
            if (args[i].equals("-o")) {
                outputFile = args[i + 1];
            }
            if (args[i].equals("-q")) {
                quiet = true;
            }
        }
        if (precision <= 0 || numThread <= 0 || args.length < 4 || args.length > 8) {
            System.out.println("ERROR: Args are not correct!!!");
        } else {
            array = new MyThread[numThread];
            BigInteger termsOfThread = BigInteger.valueOf(precision).divide(BigInteger.valueOf(4)).divide(BigInteger.valueOf(numThread));
            
            while(termsOfThread.compareTo(BigInteger.valueOf(100)) > 0) {
                termsOfThread = termsOfThread.divide(BigInteger.TEN);
            }
            
            BigInteger currentTerm = BigInteger.ZERO;
            sum = new Apfloat(0, precision);

            while (true) {
                for (int t = 0; t < numThread; t++) {
                    MyThread thread = new MyThread("Thread " + Integer.toString(t),
                            precision, numThread,
                            /*firstTerm = */ currentTerm,
                            /*lastTerm = */ currentTerm.add(termsOfThread), quiet);
                    thread.start();
                    array[t] = thread;
                    currentTerm = currentTerm.add(termsOfThread);
                }

                for (int i = 0; i < numThread; i++) {
                    try {
                        array[i].join();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    sum = sum.add(array[i].sum);
                }

                if (!quiet) {
                    System.out.println("sum = " + sum.toString());
                }
                
                // Calculate currentTerm-th term
                Apfloat nth = array[numThread - 1].lastTermValue;
                //System.out.println("next term = " + nth.toString());
                if (sum.equals(sum.add(nth))) {
                    break;
                }
            }
        }
        
        // Write to file
        try {
            PrintStream file = new PrintStream(outputFile);
            file.println(sum);
        } catch (FileNotFoundException fnf) {
            System.out.println("File " + outputFile + " not found.");
        }

        long timeOfEnd = Calendar.getInstance().getTimeInMillis();

        System.out.println("Time of calculate: " + (timeOfEnd - timeOfStart) + " ms.");
        if (!quiet) {
            System.out.println("Result: " + sum);
        }
    }

    // Evaluate n!
    public static BigInteger factorial(BigInteger n) {
        if (n.compareTo(BigInteger.ONE) <= 0) // n <= 1
            return BigInteger.ONE;
        else
            return n.multiply(factorial(n.subtract(BigInteger.ONE)));
    }
}