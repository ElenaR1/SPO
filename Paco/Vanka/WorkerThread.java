package threads;

import matrix.Matrix;
import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.concurrent.atomic.AtomicLongArray;


/**
 * Created by Niki-2014 on 1.5.2017 г..
 */
public class WorkerThread implements Runnable {
    private Matrix A;
    private Matrix B;
    private Matrix C;
    private int row;
    private int col;
    private boolean quiet = false;
    private boolean byRow = true;
    private ArrayList<Long> timesArr;

    public WorkerThread(Matrix A, Matrix B, Matrix C, int row, int col) {
        this.A = A;
        this.B = B;
        this.C = C;
        this.row = row;
        this.col = col;
        this.byRow = false;
    }

    public WorkerThread(Matrix A, Matrix B, Matrix C, int row, ArrayList<Long> timesArr) {
        this.A = A;
        this.B = B;
        this.C = C;
        this.row = row;
        this.timesArr = timesArr;
    }

    public void SetQuiet(boolean quiet) {
        this.quiet = quiet;
    }

    @Override
    public void run() {
        if(!quiet) {
            System.out.println(Thread.currentThread().getName() + " started.");
        }
        long start = System.currentTimeMillis();

        if(!byRow) {
            BigDecimal result = Matrix.Multiply(this.A.GetRow(this.row), this.B.GetColumn(this.col));
            C.SetValue(this.row, this.col, result);
        }
        else {
            for(int j = 0; j < C.GetCountCols(); j++) {
                BigDecimal value = Matrix.Multiply(this.A.GetRow(this.row), this.B.GetColumn(j));
                C.SetValue(this.row, j, value);
            }
        }

        long end = System.currentTimeMillis();
        if(!quiet) {
            System.out.println(Thread.currentThread().getName() + " stopped.");
            System.out.println(Thread.currentThread().getName() + " execution time was(millis): " + (end - start));
        }
        timesArr.add(end - start);
    }
}
