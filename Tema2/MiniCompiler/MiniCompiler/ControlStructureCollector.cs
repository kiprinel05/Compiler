using System;
using System.Collections.Generic;

public class ControlStructureCollector : MiniLangBaseVisitor<object>
{
    public List<string> ControlStructures = new List<string>();

    public override object VisitIfStatement(MiniLangParser.IfStatementContext context)
    {
        ControlStructures.Add($"if, line {context.Start.Line}");
        return base.VisitIfStatement(context);
    }

    public override object VisitWhileStatement(MiniLangParser.WhileStatementContext context)
    {
        ControlStructures.Add($"while, line {context.Start.Line}");
        return base.VisitWhileStatement(context);
    }

    public override object VisitForStatement(MiniLangParser.ForStatementContext context)
    {
        ControlStructures.Add($"for, line {context.Start.Line}");
        return base.VisitForStatement(context);
    }
}
