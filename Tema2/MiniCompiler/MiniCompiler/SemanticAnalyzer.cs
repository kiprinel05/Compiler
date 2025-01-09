using System;
using System.Collections.Generic;

public class SemanticAnalyzer : MiniLangBaseVisitor<object>
{
    public List<string> GlobalVariables = new List<string>();
    public List<string> FunctionSignatures = new List<string>();
    public List<string> Errors = new List<string>();

    private HashSet<string> currentScopeVariables = new HashSet<string>();

    public override object VisitFunction(MiniLangParser.FunctionContext context)
    {
        string name = context.IDENTIFIER().GetText();
        string returnType = context.type().GetText();
        string parameters = context.parameterList()?.GetText() ?? "none";

        string signature = $"{returnType} {name} ({parameters})";

        if (FunctionSignatures.Contains(signature))
        {
            Errors.Add($"[Semantic Error] Function '{name}' with same parameters already declared");
        }
        else
        {
            FunctionSignatures.Add(signature);
        }

        currentScopeVariables.Clear();
        if (context.parameterList() != null)
        {
            foreach (var param in context.parameterList().parameter())
            {
                string paramName = param.IDENTIFIER().GetText();
                if (currentScopeVariables.Contains(paramName))
                {
                    Errors.Add($"[Semantic Error] Duplicate parameter '{paramName}' in function '{name}'");
                }
                else
                {
                    currentScopeVariables.Add(paramName);
                }
            }
        }

        return base.VisitFunction(context);
    }

    public override object VisitVarDeclaration(MiniLangParser.VarDeclarationContext context)
    {
        string name = context.IDENTIFIER().GetText();

        if (currentScopeVariables.Contains(name) || GlobalVariables.Contains(name))
        {
            Errors.Add($"[Semantic Error] Variable '{name}' declared multiple times");
        }
        else
        {
            currentScopeVariables.Add(name);
        }

        return base.VisitVarDeclaration(context);
    }

    public override object VisitAssignment(MiniLangParser.AssignmentContext context)
    {
        string variableName = context.IDENTIFIER().GetText();

        if (!currentScopeVariables.Contains(variableName) && !GlobalVariables.Contains(variableName))
        {
            Errors.Add($"[Semantic Error] Variable '{variableName}' used before declaration");
        }

        return base.VisitAssignment(context);
    }
}
